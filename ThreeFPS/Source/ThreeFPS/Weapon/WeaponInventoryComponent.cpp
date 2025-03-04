// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponInventoryComponent.h"
#include "Character/ThreeFPSCharacter.h"
#include "GunBase.h"
#include "MovieSceneTracksComponentTypes.h"

// Sets default values for this component's properties
UWeaponInventoryComponent::UWeaponInventoryComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	CurrentWeapon = nullptr;
}

void UWeaponInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UWeaponInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UWeaponInventoryComponent::AddWeapon(EGunType GunType, AGunBase* NewWeapon)
{
	if (NewWeapon && !WeaponInventory.Contains(GunType))
	{
		WeaponInventory.Add(GunType, NewWeapon);
		NewWeapon->SetActorHiddenInGame(true);
		NewWeapon->SetOwner(GetOwner());
	}
}

void UWeaponInventoryComponent::EquipWeapon(EGunType GunType, ACharacter* Character)
{
	if (!Character || !WeaponInventory.Contains(GunType)) return;
	
	AGunBase* TempWeapon = WeaponInventory[GunType];

	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
	}
	
	CurrentWeapon = TempWeapon;
	CurrentWeapon->SetActorHiddenInGame(false);

	AThreeFPSCharacter* PlayerCharacter = Cast<AThreeFPSCharacter>(Character);
	FName SocketName = (GunType == EGunType::Rifle) ? FName("hr_weapon_rifle") : FName("hr_weapon_pistol");
	CurrentWeapon->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);

	if (PlayerCharacter && PlayerCharacter->Animations.Contains(GunType))
	{
		PlayerCharacter->GetMesh()->SetAnimInstanceClass(PlayerCharacter->Animations[GunType]);
	}
}

