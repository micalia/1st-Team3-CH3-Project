// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ThreeFPSCharacter.generated.h"

class USpringArmComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UHUDWidget;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AThreeFPSCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	// 무기 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;
	
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
	UPROPERTY(EditAnywhere,Category = "Movement")
	bool bIsSprinting;

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

	//HUD
	UPROPERTY(EditAnywhere,Category = "HUD")
	TSubclassOf<UHUDWidget> HUDClass;
	UPROPERTY()
	UHUDWidget* HUDInstance;
	
	//타이머 핸들 변수
	FTimerHandle UpdateStaminaTimer;
	
protected:
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void BeginPlay() override;
	

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

public:
	AThreeFPSCharacter();
	
	void UpdateStamina();

	//Getter 함수
	FORCEINLINE bool GetIsSprinting() const { return bIsSprinting; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE float GetCurrentStamina() const { return CurrentStamina; }
};

