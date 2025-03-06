// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Rifle.h"
#include "EGunType.h"

ARifle::ARifle()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Game/SHyeon/M4A1/Meshes/SK_M4A1"));
	if (SkeletalMesh.Succeeded())
	{
		MeshComp->SetSkeletalMesh(SkeletalMesh.Object);
	}
	GunType = EGunType::Rifle;
	bIsAuto = true;
	ReloadTime= 2.5f;
	FireRate = 0.117f;
	MeshComp->SetRelativeScale3D(FVector(0.9f, 0.9f, 0.9f));

	RecoilAmount = 0.2f;
	RecoilRandomness = 0.5f;
	AimRecoilMultiplier =0.3f;
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();
}

void ARifle::Fire()
{
	Super::Fire();
}

void ARifle::StartFire()
{
	Super::StartFire();
}

void ARifle::StopFire()
{
	Super::StopFire();
}

void ARifle::StartReload()
{
	Super::StartReload();
}

void ARifle::OnReloaded()
{
	Super::OnReloaded();
}

void ARifle::ApplyRecoil()
{
	Super::ApplyRecoil();
}

bool ARifle::CanFire() const
{
	return Super::CanFire();
}

bool ARifle::CanReloading() const
{
	return Super::CanReloading();
}

void ARifle::PlayFireAnim()
{
	Super::PlayFireAnim();
}
