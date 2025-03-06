// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "GunBase.generated.h"


class AThreeFPSCharacter;
enum class EGunType : uint8;
class UCurveFloat;

UCLASS()
class THREEFPS_API AGunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AGunBase();
	
protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	USkeletalMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Root")
	USceneComponent* Root;
	
	
	/* 총 속성들 */
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	float Damage;
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	float FireRate;
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	float ReloadTime;
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	int MaxAmmo;
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	int MagazineSize;
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	int CurrentAmmo;
	UPROPERTY(VisibleAnywhere, Category = "GunProperties")
	EGunType GunType;

	/* bool 변수들 */
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite ,Category = "GunProperties")
	bool bIsFiring;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite ,Category = "GunProperties")
	bool bIsReloading;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite ,Category = "GunProperties")
	bool bIsAuto;

	/* 타이머 핸들 */
	FTimerHandle AutoFireTimer;
	FTimerHandle ReloadTimer;
	
	/* 타임 라인 */
	FTimeline RecoilTimeLine;

	//사격 이펙트 및 애니메이션
	UPROPERTY(EditAnywhere, Category = "GunEffect")
	TArray<USoundBase*> FireSounds;
	UPROPERTY(EditAnywhere, Category = "GunEffect")
	USoundBase* ClickSound;
	UPROPERTY(EditAnywhere, Category = "GunEffect")
	UParticleSystem* FireParticle;
	UPROPERTY(EditAnywhere, Category = "GunEffect")
	UParticleSystem* WallParticle;
	UPROPERTY(EditAnywhere, Category = "GunEffect")
	UAnimMontage* FireMontage;
	UPROPERTY(EditAnywhere, Category = "GunEffect")
	UAnimMontage* ReloadMontage;

	/* 반동 변수 */
	float RecoilAmount;
	float RecoilRandomness;
	float AimRecoilMultiplier;

	//발사 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	UAnimMontage* HipFireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	UAnimMontage* AimFireMontage;
	
public:
	
	virtual void Fire();
	UFUNCTION(BlueprintCallable,Category="Fire")
	virtual void StartFire();
	UFUNCTION(BlueprintCallable,Category="Fire")
	virtual void StopFire();
	
	virtual void StartReload();
	virtual void OnReloaded();
	virtual void ApplyRecoil();

	virtual void AddMaxAmmo(int32 Amount);
	
	virtual bool CanFire() const;
	virtual bool CanReloading() const;
	virtual void PlayFireAnim();
	FORCEINLINE virtual float GetRecoil() const {return RecoilAmount;}
	FORCEINLINE virtual bool IsReloading() const { return bIsReloading; }
	FORCEINLINE virtual bool IsAuto() const { return bIsAuto; }
	FORCEINLINE virtual bool IsFiring() const { return bIsFiring; }
	FORCEINLINE virtual float GetFireRate() const { return FireRate; }
	FORCEINLINE virtual float GetReloadTime() const { return ReloadTime; }
	FORCEINLINE virtual int GetCurrentAmmo() const { return CurrentAmmo; }
	FORCEINLINE virtual int GetMaxAmmo() const { return MaxAmmo; }
	FORCEINLINE virtual int GetMagazineSize() const { return MagazineSize; }
	FORCEINLINE virtual EGunType GetGunType() const { return GunType; }
};
