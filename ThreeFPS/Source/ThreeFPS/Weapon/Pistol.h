// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GunBase.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class THREEFPS_API APistol : public AGunBase
{
	GENERATED_BODY()

public:
	APistol();
	
	virtual void Fire() override;
	virtual void StartFire() override;
	virtual void StopFire() override;
	
	virtual void StartReload() override;
	virtual void OnReloaded() override;
	virtual void ApplyRecoil() override;
	
	virtual bool CanFire() const override;
	virtual bool CanReloading() const override;
	FORCEINLINE virtual float GetRecoil() const override {return RecoilAmount;}
	FORCEINLINE virtual bool IsReloading() const override { return bIsReloading; }
	FORCEINLINE virtual bool IsAuto() const override { return bIsAuto; }
	FORCEINLINE virtual bool IsFiring() const override { return bIsFiring; }
	FORCEINLINE virtual float GetFireRate() const override { return FireRate; }
	FORCEINLINE virtual float GetReloadTime() const override { return ReloadTime; }
	FORCEINLINE virtual int GetCurrentAmmo() const override { return CurrentAmmo; }
	FORCEINLINE virtual int GetMaxAmmo() const override { return MaxAmmo; }
	FORCEINLINE virtual int GetMagazineSize() const override { return MagazineSize; }
	FORCEINLINE virtual EGunType GetGunType() const override { return GunType; }
};
