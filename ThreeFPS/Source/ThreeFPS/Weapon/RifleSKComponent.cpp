// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RifleSKComponent.h"

URifleSKComponent::URifleSKComponent()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RifleMesh(TEXT("/Game/SHyeon/M4A1/Meshes/SK_M4A1.SK_M4A1)"));
	if (RifleMesh.Succeeded())
	{
		this->SetSkeletalMesh(RifleMesh.Object);
	}
}

void URifleSKComponent::Fire()
{
	Super::Fire();
}

void URifleSKComponent::StartFire()
{
	Super::StartFire();
}

void URifleSKComponent::StopFire()
{
	Super::StopFire();
}

void URifleSKComponent::StartReload()
{
	Super::StartReload();
}

void URifleSKComponent::OnReloaded()
{
	Super::OnReloaded();
}
