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
                      GunType(),bIsFiring(false), bIsReloading(false),bIsAuto(true), FireParticle(nullptr), FireMontage(nullptr), ReloadMontage(nullptr)
                     
{
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	MeshComp =CreateDefaultSubobject<USkeletalMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(Root);
}

void AGunBase::BeginPlay()
{
	Super::BeginPlay();

	// FOnTimelineFloat XRecoilCurve;
	// FOnTimelineFloat YRecoilCurve;
	//
	// XRecoilCurve.BindUFunction(this, FName("StartHorizontalRecoil"));
	// YRecoilCurve.BindUFunction(this, FName("StartVerticalRecoil"));

	// if (HorizontalCurve && VerticalCurve)
	// {
	// 	RecoilTimeLine.AddInterpFloat(HorizontalCurve, XRecoilCurve);
	// 	RecoilTimeLine.AddInterpFloat(VerticalCurve, YRecoilCurve);
	// }
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

void AGunBase::Fire()
{
	if (CurrentAmmo <= 0 || bIsReloading)
	{
		if (ClickSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ClickSound, GetActorLocation());
		}
		// AThreeFPSCharacter* Character = Cast<AThreeFPSCharacter>(GetOwner());
		// if (Character)
		// {
		// 	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		// 	if (AnimInstance)
		// 	{
		// 		AnimInstance->Montage_Stop(0.1f);
		// 	}
		// 	return;
		// }
		return;
	}
	FVector MuzzleLocation = MeshComp->GetSocketLocation(FName("Muzzle"));
	FRotator MuzzleRotation = MeshComp->GetSocketRotation(FName("Muzzle"));
	FVector TraceEnd;
	
	if (FireParticle)
	{
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireParticle, MuzzleLocation, MuzzleRotation);
	}
	if (FireSounds.Num()>0)
	{
		int16 RandSoundSum = FMath::RandRange(0, FireSounds.Num()-1);
		USoundBase* FireSound = FireSounds[RandSoundSum];
		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, MuzzleLocation);
		}
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
	QueryParams.AddIgnoredActor(GetOwner());
	
	if (bool Hit = GetWorld()->LineTraceSingleByChannel(HitResult,MuzzleLocation, TraceEnd,ECC_Pawn, QueryParams))
	{
		FVector SurfaceNormal = HitResult.Normal;
		FRotator Rotator = SurfaceNormal.Rotation();
		FVector EffectScale = FVector(1.2f, 1.2f, 1.2f);
		
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
			if (WallParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WallParticle, TraceEnd, Rotator, EffectScale);
			}
		}
		else if (HitActor)
		{
			UGameplayStatics::ApplyDamage(HitResult.GetActor(),Damage, PlayerController, GetOwner(),  UDamageType::StaticClass());
			//디버깅용 메세지
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("액터 : %s"), *HitActor->GetName()));
			if (WallParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WallParticle, TraceEnd, Rotator, EffectScale);
			}
		}
			
	}
	DrawDebugLine(GetWorld(), MuzzleLocation, TraceEnd, FColor::Red, false, 1.f,0,2.f);
	CurrentAmmo--;
	
	if (AThreeFPSCharacter* Character = Cast<AThreeFPSCharacter>(GetOwner()))
	{
		Character->UpdateAmmo();
	}
	
	UE_LOG(LogTemp, Display, TEXT("Current Ammo : %d"), CurrentAmmo);
	// PlayFireAnimation(bIsAiming);
	ApplyRecoil();
}

void AGunBase::StartFire()
{
	if (bIsReloading) return;
	if (bIsAuto)
	{
		GetWorldTimerManager().SetTimer(AutoFireTimer, this, &AGunBase::Fire, FireRate, true);
	}
	Fire();
}

void AGunBase::StopFire()
{
	if (bIsAuto)
	{
		GetWorldTimerManager().ClearTimer(AutoFireTimer);
	}
}

void AGunBase::PlayFireAnimation(bool bIsAiming)
{
	// 총을 장착한 캐릭터 가져오기
	AThreeFPSCharacter* Character = Cast<AThreeFPSCharacter>(GetOwner());
	if (!Character) return;

	// 캐릭터의 애니메이션 인스턴스 가져오기
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	// 사용할 몽타주 선택
	UAnimMontage* FireMontageToPlay = bIsAiming ? AimFireMontage : HipFireMontage;

	// 몽타주가 유효하면 실행
	if (FireMontageToPlay)
	{
		AnimInstance->Montage_Play(FireMontageToPlay, FireRate, EMontagePlayReturnType::MontageLength,0.f,true);
	}
}

void AGunBase::StartReload()
{
	if (CurrentAmmo == MagazineSize || MaxAmmo <= 0) return;
	
	bIsReloading = true;
	if (ReloadMontage)
	{
		MeshComp->PlayAnimation(ReloadMontage, false);
	}
	
	int NeededAmmo = MagazineSize - CurrentAmmo;
	if (MaxAmmo <= NeededAmmo)
	{
		CurrentAmmo += MaxAmmo;
		MaxAmmo = 0;
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

void AGunBase::ApplyRecoil()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		AThreeFPSCharacter* Character = Cast<AThreeFPSCharacter>(PlayerController->GetPawn());
		if (Character)
		{
			
			float FinalRecoilAmount = RecoilAmount;
			float FinalRecoilRandomness = RecoilRandomness;
			
			// 조준 반동 
			if (Character->GetIsAiming())
			{
				FinalRecoilAmount *= AimRecoilMultiplier;
				FinalRecoilRandomness *= AimRecoilMultiplier;
			}
			
			float PitchRecoil = FMath::RandRange(FinalRecoilAmount * 0.8f, FinalRecoilAmount * 1.2f);
			float YawRecoil = FMath::RandRange(-FinalRecoilRandomness, FinalRecoilRandomness);

			PlayerController->AddPitchInput(-PitchRecoil);
			PlayerController->AddYawInput(YawRecoil);
		}
	}
}

bool AGunBase::CanFire() const
{
	if (CurrentAmmo <= 0 || bIsReloading) return false;
	return true;
}

bool AGunBase::CanReloading() const
{
	if (CurrentAmmo == MagazineSize || MaxAmmo <= 0) return false;
	return true; 
}
