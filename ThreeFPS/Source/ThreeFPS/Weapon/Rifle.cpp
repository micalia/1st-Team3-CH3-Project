// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Rifle.h"
#include "EGunType.h"
#include "SkeletonTreeBuilder.h"

ARifle::ARifle()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Game/SHyeon/M4A1/Meshes/SK_M4A1"));
	if (SkeletalMesh.Succeeded())
	{
		MeshComp->SetSkeletalMesh(SkeletalMesh.Object);
	}
	GunType = EGunType::Rifle;
	bIsAuto = true;
	ReloadTime= 2.25f;
	FireRate = 0.117f;
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();
}

void ARifle::StartHorizontalRecoil(float Value)
{
	Super::StartHorizontalRecoil(Value);
}

void ARifle::StartVerticalRecoil(float Value)
{
	Super::StartVerticalRecoil(Value);
}

void ARifle::StartRecoil()
{
	Super::StartRecoil();
}

void ARifle::ReverseRecoil()
{
	Super::ReverseRecoil();
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

bool ARifle::CanReloading() const
{
	return Super::CanReloading();
}
