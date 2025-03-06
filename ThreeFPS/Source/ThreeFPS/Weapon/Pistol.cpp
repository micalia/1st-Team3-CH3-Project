// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Pistol.h"

APistol::APistol()
{
	bIsAuto = false;
	MaxAmmo = INFINITY;
	MagazineSize = 15;
	CurrentAmmo = MagazineSize;
	RecoilAmount = 0.7f;
	RecoilRandomness = 0.8f;
	AimRecoilMultiplier =0.4f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Game/SHyeon/Pistol/Props/SK_DE_50_C_Skeletal"));
	if (SkeletalMesh.Succeeded())
	{
		MeshComp->SetSkeletalMesh(SkeletalMesh.Object);
	}
}

void APistol::Fire()
{
	Super::Fire();
}

void APistol::StartFire()
{
	Super::StartFire();
}

void APistol::StopFire()
{
	Super::StopFire();
}


void APistol::StartReload()
{
	Super::StartReload();
}

void APistol::OnReloaded()
{
	Super::OnReloaded();
}

void APistol::ApplyRecoil()
{
	Super::ApplyRecoil();
}

bool APistol::CanFire() const
{
	return Super::CanFire();
}

bool APistol::CanReloading() const
{
	return Super::CanReloading();
}
