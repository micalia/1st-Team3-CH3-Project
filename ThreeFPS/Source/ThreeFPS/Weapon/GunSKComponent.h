// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "EGunType.h"
#include "GunSKComponent.generated.h"

UCLASS()
class THREEFPS_API UGunSKComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
protected:
	/* 총 속성들 */
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	float Damage;
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	float FireRate;
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	float ReloadTime;
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	uint16 MaxAmmo;
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	uint8 MagazineSize;
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	uint8 CurrentAmmo;
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	EGunType GunType;

	/* bool 변수들 */
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	bool bIsFiring;
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	bool bIsReloading;
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	bool bIsAuto;

	/* 타이머 핸들 */
	FTimerHandle AutoFireTimer;
	FTimerHandle ReloadTimer;

	//사격 이펙트 및 애니메이션
	UPROPERTY(EditAnywhere, Category = "GunEffect")
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, Category = "GunEffect")
	UParticleSystem* FireParticle;
	UPROPERTY(EditAnywhere, Category = "GunEffect")
	UAnimMontage* FireMontage;
	UPROPERTY(EditAnywhere, Category = "GunEffect")
	UAnimMontage* ReloadMontage;
	
public:
	UGunSKComponent();
	
	virtual void Fire();
	virtual void StartFire();
	virtual void StopFire();
	
	virtual void StartReload();
	virtual void OnReloaded();

	FORCEINLINE virtual bool IsReloading() const { return bIsReloading; }
	FORCEINLINE virtual bool IsAuto() const { return bIsAuto; }
	FORCEINLINE virtual bool IsFiring() const { return bIsFiring; }
	FORCEINLINE virtual float GetFireRate() const { return FireRate; }
	FORCEINLINE virtual float GetReloadTime() const { return ReloadTime; }
	FORCEINLINE virtual uint8 GetCurrentAmmo() const { return CurrentAmmo; }
	FORCEINLINE virtual uint8 GetMaxAmmo() const { return MaxAmmo; }
	FORCEINLINE virtual uint8 GetMagazineSize() const { return MagazineSize; }
	FORCEINLINE virtual EGunType GetGunType() const { return GunType; }
};
