// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Components/TimelineComponent.h"
#include "Weapon/EGunType.h"
#include "ThreeFPSCharacter.generated.h"

class UWeaponInventoryComponent;
enum class EGunType : uint8;
class UThreeFPSUIComponent;
class USpringArmComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UHUDWidget;
class AGunBase;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AThreeFPSCharacter : public ACharacter
{
	GENERATED_BODY()
	
	/** camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	//3인칭 카메라
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCameraComponent;
	//스프링 암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	//속도 변수
	UPROPERTY(EditAnywhere, Category = "Movement")
	float OriginSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed;
	UPROPERTY(EditAnywhere,Category = "Movement")
	float SprintRate;
	UPROPERTY(EditAnywhere,Category = "Movement", meta= (AllowPrivateAccess = "true"))
	bool bIsSprinting;
	UPROPERTY(EditAnywhere,Category = "Movement")
	bool bShouldMove;

	//체력, 스태미너 변수
	UPROPERTY(EditAnywhere,Category = "Status")
	float MaxHealth;
	UPROPERTY(EditAnywhere,Category = "Status")
	float CurrentHealth;
	UPROPERTY(EditAnywhere,Category = "Status")
	float MaxStamina;
	UPROPERTY(EditAnywhere,Category = "Status")
	float CurrentStamina;
	UPROPERTY(EditAnywhere,Category = "Status")
	float StaminaConsumeRate;
	UPROPERTY(EditAnywhere,Category = "Status")
	float StaminaRegenRate;
	UPROPERTY(EditAnywhere,Category = "Status")
	bool bIsStaminaEmpty;

	//조준, 발사
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Fire", meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Fire", meta = (AllowPrivateAccess = "true"))
	float OriginSpringArmLength;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Fire", meta = (AllowPrivateAccess = "true"))
	float AimedSpringArmLength;
	UPROPERTY(EditAnywhere, Category = "Fire")
	bool bIsFiring;

	
	//재장전
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Fire", meta = (AllowPrivateAccess = "true"))
	bool bIsReloading;
	
	//HUD
	UPROPERTY(EditAnywhere,Category = "HUD")
	TSubclassOf<UHUDWidget> HUDClass;
	UPROPERTY()
	UHUDWidget* HUDInstance;
	
	//크로스헤어용 UI컴포넌트.
	UPROPERTY(visibleAnywhere, Category = "Movement")
	UThreeFPSUIComponent* UIComponent;
	
	//타이머 핸들 변수
	FTimerHandle UpdateStaminaTimer;
	FTimerHandle FireTimer;
	FTimerHandle ReloadTimer;
	
protected:

	//인벤토리 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess=true))
	UWeaponInventoryComponent* WeaponInventory;
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<AGunBase> RifleClass;

	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//사망
	void Die();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	//달리기 함수
	void StartSprint();
	void StopSprint();
	//앉기 함수
	void StartCrouch();
	void StopCrouch();
	//조준 함수
	void StartAim();
	void StopAim();
	void UpdateAimProgress(float Value);
	//발사 함수
	void StartFiring();
	void StopFiring();
	//재장전
	void StartReload();
	void OnReloaded();

	void EquipRifle();
	void EquipPistol();
public:
	FTimeline AimTimeLine;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Aim")
	UCurveFloat* AimCurve;
	
	AThreeFPSCharacter();
	
	void GameStart();
	void UpdateStamina();
	void UpdateHP();
	
	//Getter 함수
	FORCEINLINE bool GetIsAiming() { return bIsAiming; }
	FORCEINLINE bool GetIsSprinting() const { return bIsSprinting; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE float GetCurrentStamina() const { return CurrentStamina; }
	
	//무기에 따른 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	TMap<EGunType, TSubclassOf<UAnimInstance>> Animations;
};


