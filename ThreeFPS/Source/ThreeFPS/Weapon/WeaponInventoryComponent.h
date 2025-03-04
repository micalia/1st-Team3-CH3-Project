// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponInventoryComponent.generated.h"


enum class EGunType : uint8;
class AGunBase;
class AThreeFPSCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THREEFPS_API UWeaponInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	AGunBase* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	TMap<EGunType, AGunBase*> WeaponInventory;
	
public:
	UWeaponInventoryComponent();

	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void AddWeapon(EGunType GunType, AGunBase* Weapon);
	void EquipWeapon(EGunType GunType, ACharacter* Character);

	FORCEINLINE AGunBase* GetCurrentWeapon() {return CurrentWeapon;}
};
