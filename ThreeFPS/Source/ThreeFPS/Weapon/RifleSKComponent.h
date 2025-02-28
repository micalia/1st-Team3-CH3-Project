// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GunSKComponent.h"
#include "RifleSKComponent.generated.h"

/**
 * 
 */
UCLASS()
class THREEFPS_API URifleSKComponent : public UGunSKComponent
{
	GENERATED_BODY()
	
public:
	URifleSKComponent();

	virtual void Fire() override;
	virtual void StartFire() override;
	virtual void StopFire() override;
	
	virtual void StartReload() override;
	virtual void OnReloaded() override;

	FORCEINLINE virtual bool IsReloading() const override { return bIsReloading; }
	FORCEINLINE virtual bool IsAuto() const override{ return bIsAuto; }
	FORCEINLINE virtual bool IsFiring() const override{ return bIsFiring; }
	FORCEINLINE virtual float GetFireRate() const override{ return FireRate; }
	FORCEINLINE virtual float GetReloadTime() const override{ return ReloadTime; }
	FORCEINLINE virtual uint8 GetCurrentAmmo() const override{ return CurrentAmmo; }
	FORCEINLINE virtual uint8 GetMaxAmmo() const override{ return MaxAmmo; }
	FORCEINLINE virtual uint8 GetMagazineSize() const override{ return MagazineSize; }
	FORCEINLINE virtual EGunType GetGunType() const override{ return GunType; }
};
