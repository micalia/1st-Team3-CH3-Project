// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GunSKComponent.h"
#include "Game/ThreeFPSPlayerController.h"
#include "Kismet/GameplayStatics.h"

UGunSKComponent::UGunSKComponent() : Damage(10.f), FireRate(0.1f), ReloadTime(1.25f), MaxAmmo(300), MagazineSize(30), CurrentAmmo(MagazineSize),
                                     GunType(),bIsFiring(false), bIsReloading(false),bIsAuto(true), FireSound(nullptr), FireParticle(nullptr), FireMontage(nullptr), ReloadMontage(nullptr) {}


void UGunSKComponent::Fire()
{
	if (CurrentAmmo <= 0 || bIsReloading) return;
	
	CurrentAmmo--;
	
	FVector MuzzleLocation = GetSocketLocation(FName("Muzzle"));
	FRotator MuzzleRotation = GetSocketRotation(FName("Muzzle"));
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
	QueryParams.AddIgnoredComponent(this);

	if (bool Hit = GetWorld()->LineTraceSingleByChannel(HitResult,MuzzleLocation,TraceEnd,ECC_Pawn, QueryParams))
	{
		TraceEnd = HitResult.ImpactPoint;
		if (AActor* HitActor = HitResult.GetActor())
		{
			UGameplayStatics::ApplyDamage(HitResult.GetActor(),Damage, PlayerController, GetOwner(),  UDamageType::StaticClass());
			//디버깅용 메세지
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("액터 : %s"), *HitActor->GetName()));
		}
	}
	DrawDebugLine(GetWorld(), MuzzleLocation, TraceEnd, FColor::Red, false, 1.f,0,2.f);
}


void UGunSKComponent::StartFire()
{
	if (bIsReloading) return;
	if (bIsAuto)
	{
		GetWorld()->GetTimerManager().SetTimer(AutoFireTimer, this, &UGunSKComponent::Fire, FireRate, true);
	}
}

void UGunSKComponent::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(AutoFireTimer);
}

void UGunSKComponent::StartReload()
{
	if (CurrentAmmo == MagazineSize || MaxAmmo <= 0) return;
	
	bIsReloading = true;
	if (ReloadMontage)
	{
		PlayAnimation(ReloadMontage, "reload");
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
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &UGunSKComponent::OnReloaded, ReloadTime, false);
}

void UGunSKComponent::OnReloaded()
{
	bIsReloading = false;
}
