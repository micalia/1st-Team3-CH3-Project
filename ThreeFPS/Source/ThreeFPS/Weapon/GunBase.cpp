// Fill out your copyright notice in the Description page of Project Settings.
#include "Weapon/GunBase.h"
#include "Game/ThreeFPSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "EGunType.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "Character/ThreeFPSCharacter.h"
#include "Mission/TutorialTarget.h"

AGunBase::AGunBase(): Damage(10.f), FireRate(0.1f), ReloadTime(2.25f), MaxAmmo(300), MagazineSize(30), CurrentAmmo(MagazineSize),
                      GunType(),bIsFiring(false), bIsReloading(false),bIsAuto(true), FireSound(nullptr), FireParticle(nullptr), FireMontage(nullptr), ReloadMontage(nullptr)
                     
{
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	MeshComp =CreateDefaultSubobject<USkeletalMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(Root);
}

void AGunBase::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat XRecoilCurve;
	FOnTimelineFloat YRecoilCurve;

	XRecoilCurve.BindUFunction(this, FName("StartHorizontalRecoil"));
	YRecoilCurve.BindUFunction(this, FName("StartVerticalRecoil"));

	if (HorizontalCurve && VerticalCurve)
	{
		RecoilTimeLine.AddInterpFloat(HorizontalCurve, XRecoilCurve);
		RecoilTimeLine.AddInterpFloat(VerticalCurve, YRecoilCurve);
	}
}

void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (RecoilTimeLine.IsPlaying())
	{
		RecoilTimeLine.TickTimeline(DeltaTime);
	}
	if (RecoilTimeLine.IsReversing())
	{
		RecoilTimeLine.TickTimeline(-DeltaTime);
	}
}

void AGunBase::StartHorizontalRecoil(float Value)
{
	if (AThreeFPSPlayerController* PlayerController = Cast<AThreeFPSPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		AThreeFPSCharacter* Character = Cast<AThreeFPSCharacter>(PlayerController->GetPawn());
		if (Character)
		{
			Character->AddControllerYawInput(Value);
		}
	}	
}

void AGunBase::StartVerticalRecoil(float Value)
{
	if (AThreeFPSPlayerController* PlayerController = Cast<AThreeFPSPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		AThreeFPSCharacter* Character = Cast<AThreeFPSCharacter>(PlayerController->GetPawn());
		if (Character)
		{
			Character->AddControllerPitchInput(Value);
		}
	}	
}

void AGunBase::StartRecoil()
{
	RecoilTimeLine.PlayFromStart();
}

void AGunBase::ReverseRecoil()
{
	RecoilTimeLine.Reverse();
}


void AGunBase::Fire()
{
	if (CurrentAmmo <= 0 || bIsReloading)
	{
		if (ClickSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ClickSound, GetActorLocation());
		}
		return;
	}
	
	FVector MuzzleLocation = MeshComp->GetSocketLocation(FName("Muzzle"));
	FRotator MuzzleRotation = MeshComp->GetSocketRotation(FName("Muzzle"));
	FVector TraceEnd;
	
	if (FireParticle)
	{
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireParticle, MuzzleLocation, MuzzleRotation);
	}
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleLocation);
	}

	AThreeFPSPlayerController* PlayerController = nullptr;
	if (APawn* owner = Cast<APawn>(GetOwner()))
	{
		PlayerController = Cast<AThreeFPSPlayerController>(owner->GetController());
		if (PlayerController)
		{
			FRotator CameraRotator;
			FVector CameraLocation;
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotator);

			FVector ShotDirection = CameraRotator.Vector();
			TraceEnd = CameraLocation + (ShotDirection* 5000.f);
		}
	}
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (bool Hit = GetWorld()->LineTraceSingleByChannel(HitResult,MuzzleLocation,TraceEnd,ECC_Pawn, QueryParams))
	{
		TraceEnd = HitResult.ImpactPoint;
		AActor* HitActor = HitResult.GetActor();
		if (HitActor->ActorHasTag("Zombie"))
		{
			FPointDamageEvent PointDamageEvent;
			PointDamageEvent.HitInfo = HitResult; // HitResult를 통해 정확한 충돌 정보 전달
			UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), 20.f, TraceEnd,
				PointDamageEvent.HitInfo, PlayerController, this,
				UDamageType::StaticClass());
		}
		/*튜토리얼 과녁 쓰러지는 이벤트 - 설빈 추가*/
		else if (auto Target = Cast<ATutorialTarget>(HitActor)) {
			Target->OnHitTarget();
		}
		else if (HitActor)
		{
			UGameplayStatics::ApplyDamage(HitResult.GetActor(),Damage, PlayerController, GetOwner(),  UDamageType::StaticClass());
			//디버깅용 메세지
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("액터 : %s"), *HitActor->GetName()));
		}
	}
	DrawDebugLine(GetWorld(), MuzzleLocation, TraceEnd, FColor::Red, false, 1.f,0,2.f);
	StartRecoil();
	CurrentAmmo--;
}


void AGunBase::StartFire()
{
	if (bIsReloading) return;
	if (bIsAuto)
	{
		StartRecoil();
		GetWorldTimerManager().SetTimer(AutoFireTimer, this, &AGunBase::Fire, FireRate, true);
	}
	Fire();
}

void AGunBase::StopFire()
{
	if (bIsAuto)
	{
		GetWorldTimerManager().ClearTimer(AutoFireTimer);
		ReverseRecoil();
	}
	ReverseRecoil();
}

void AGunBase::StartReload()
{
	if (CurrentAmmo == MagazineSize || MaxAmmo <= 0) return;
	
	bIsReloading = true;
	if (ReloadMontage)
	{
		MeshComp->PlayAnimation(ReloadMontage, false);
	}
	
	int8 NeededAmmo = MagazineSize - CurrentAmmo;
	if (MaxAmmo <= NeededAmmo)
	{
		MaxAmmo = 0;
		CurrentAmmo += MaxAmmo;
	}
	else
	{
		MaxAmmo -= NeededAmmo;
		CurrentAmmo = MagazineSize;
	}
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AGunBase::OnReloaded, ReloadTime, false);
}

void AGunBase::OnReloaded()
{
	bIsReloading = false;
	
}

bool AGunBase::CanReloading() const
{
	if (CurrentAmmo == MagazineSize || MaxAmmo <= 0) return false;
	return true; 
}
