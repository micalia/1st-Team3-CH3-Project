// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThreeFPSCharacter.h"
#include "Weapon/WeaponInventoryComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Game/ThreeFPSPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HUDWidget.h"
#include "OnDiedWidget.h"
#include "ThreeFPSUIComponent.h"
#include "InputActionValue.h"
#include "Inventory/InventoryWidget.h"
#include "MovieSceneTracksComponentTypes.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "Weapon/EGunType.h"
#include "Weapon/Rifle.h"
#include <Item/ItemBase.h>

#include "UniversalObjectLocators/AnimInstanceLocatorFragment.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AThreeFPSCharacter::AThreeFPSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	//UI컴포넌트 부착
	UIComponent = CreateDefaultSubobject<UThreeFPSUIComponent>(TEXT("UIComponent"));
	
	//인벤토리 컴포넌트 부착
	WeaponInventory = CreateDefaultSubobject<UWeaponInventoryComponent>(TEXT("WeaponInventory"));
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	//3인칭 카메라 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->TargetArmLength = 250.f;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	SpringArm->SetRelativeRotation(FRotator(0, -90.f, 0));
	SpringArm->bUsePawnControlRotation = true;
	
	ThirdPersonCameraComponent=CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCameraComponent->SetupAttachment(SpringArm);
	ThirdPersonCameraComponent->SetActive(true);
	ThirdPersonCameraComponent->bUsePawnControlRotation = false;
	
	//3인칭 시점 메쉬 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/Meshes/TwinBlast"));
	if (MeshAsset.Succeeded())
	{
 		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0.f));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));

	//에임 시 카메라
	AimedSpringArmLength = 100.f;
	OriginSpringArmLength = 250.f;
	
	//앉기 활성화
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	
	//달리기 변수 초기화.
	OriginSpeed = GetCharacterMovement()->MaxWalkSpeed;
	SprintRate = 1.5f;
	SprintSpeed = OriginSpeed * SprintRate;
	bIsSprinting = false;
	
	//체력 스태미너
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
	MaxStamina = 100.f;
	CurrentStamina = MaxStamina;
	bIsStaminaEmpty = false;
	StaminaConsumeRate = 0.5f;
	StaminaRegenRate = 0.3f;
	
	// 돌연변이
	MaxMutation = 200.f;
	MutationRate = 1.f;
	CurrentMutation = 0.f;
	
	//HUD
	HUDClass =nullptr;
	HUDInstance = nullptr;
	GameOverHUDClass = nullptr;

	//발사 변수
	bIsFiring=false;

	/* 장애물 회피를 위한 중력 조정 - 설빈 추가*/
	GetCharacterMovement()->JumpZVelocity = 1350.f;
	GetCharacterMovement()->GravityScale = 4.f;

	bCanJump = true;
	bIsDive = false;
}

void AThreeFPSCharacter::IncreaseHealth(int32 Amount)
{ 
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth);
	UpdateHP();
}

void AThreeFPSCharacter::DecreaseMutation(int32 Amount)
{
	CurrentMutation = FMath::Clamp(CurrentMutation - Amount, 0.f, MaxMutation);
	UpdateMutation();
}

void AThreeFPSCharacter::IncreaseAmmo(int32 Amount)
{
	WeaponInventory->GetCurrentWeapon()->AddMaxAmmo(Amount);
	UpdateAmmo();
}

// Input
void AThreeFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AThreeFPSPlayerController* PlayerController = Cast<AThreeFPSPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction){EnhancedInputComponent->BindAction(PlayerController->MoveAction,ETriggerEvent::Triggered,this, &AThreeFPSCharacter::Move);}
			if (PlayerController->LookAction){EnhancedInputComponent->BindAction(PlayerController->LookAction,ETriggerEvent::Triggered,this, &AThreeFPSCharacter::Look);}
			if (PlayerController->JumpAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->JumpAction,ETriggerEvent::Triggered,this, &AThreeFPSCharacter::Jump);
			}
			if (PlayerController->CrouchAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->CrouchAction,ETriggerEvent::Started,this, &AThreeFPSCharacter::StartCrouch);
				EnhancedInputComponent->BindAction(PlayerController->CrouchAction,ETriggerEvent::Completed,this, &AThreeFPSCharacter::StopCrouch);
			}
			if (PlayerController->SprintAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->SprintAction,ETriggerEvent::Started,this, &AThreeFPSCharacter::StartSprint);
				EnhancedInputComponent->BindAction(PlayerController->SprintAction,ETriggerEvent::Completed,this, &AThreeFPSCharacter::StopSprint);
			}
			if (PlayerController->AimAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->AimAction,ETriggerEvent::Started, this, &AThreeFPSCharacter::StartAim);
				EnhancedInputComponent->BindAction(PlayerController->AimAction,ETriggerEvent::Completed, this, &AThreeFPSCharacter::StopAim);
			}
			if (PlayerController->FireAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->FireAction,ETriggerEvent::Started, this, &AThreeFPSCharacter::StartFiring);
				EnhancedInputComponent->BindAction(PlayerController->FireAction,ETriggerEvent::Completed, this, &AThreeFPSCharacter::StopFiring);
			}
			if (PlayerController->InteractAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->InteractAction, ETriggerEvent::Started, this, &AThreeFPSCharacter::Interact);
			}
			if (PlayerController->InventoryAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->InventoryAction, ETriggerEvent::Started, this, &AThreeFPSCharacter::ToggleInventory);
			}
			if (PlayerController->ReloadAction) EnhancedInputComponent->BindAction(PlayerController->ReloadAction, ETriggerEvent::Triggered,this, &AThreeFPSCharacter::StartReload);
			if (PlayerController->EquipRifleAction) EnhancedInputComponent->BindAction(PlayerController->EquipRifleAction, ETriggerEvent::Triggered, this, &AThreeFPSCharacter::EquipRifle);
			if (PlayerController->DiveAction) EnhancedInputComponent->BindAction(PlayerController->DiveAction, ETriggerEvent::Triggered, this, &AThreeFPSCharacter::StartDive);
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AThreeFPSCharacter::GameStart()
{
	if (HUDClass)
	{
		AThreeFPSPlayerController* PlayerController = Cast<AThreeFPSPlayerController>(GetController());
		HUDInstance = CreateWidget<UHUDWidget>(PlayerController, HUDClass);
		HUDInstance->AddToViewport(1);

		if (APawn* PlayerPawn = PlayerController->GetPawn())
		{
			PlayerController->SetViewTargetWithBlend(PlayerPawn, 1.0f);
		}
	}
	
	UIComponent->GameStart();
	bIsDead = false;
	
	//무기 인벤에 추가
	if (RifleClass)
	{
		if (AGunBase* Rifle = GetWorld()->SpawnActor<AGunBase>(RifleClass))
		{
			WeaponInventory->AddWeapon(EGunType::Rifle, Rifle);
		}
	}
	
	//라이플 장착
	EquipRifle();
	
	//스프링암 타임라인
	// if (AimCurve)
	// {
	// 	FOnTimelineFloat TimeLineProgress;
	// 	TimeLineProgress.BindUFunction(this, FName("UpdateAimProgress"));
	// 	AimTimeLine.AddInterpFloat(AimCurve, TimeLineProgress);
	// }
	
	//체력 UI업데이트
	UpdateHP();
	//스테미너 업데이트 타이머
	GetWorldTimerManager().SetTimer(UpdateStaminaTimer, this, &AThreeFPSCharacter::UpdateStamina, 0.1f, true);
	GetWorldTimerManager().SetTimer(UpdateMutationTimer, this, &AThreeFPSCharacter::UpdateMutation, 1.f, true);
	//탄약 Text블록
	UpdateAmmo();
}

//비긴 플레이
void AThreeFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	InventoryWidget = CreateWidget<UInventoryWidget>(Cast<APlayerController>(GetController()), InventoryWidgetClass);
	InteractWidget = CreateWidget(Cast<APlayerController>(GetController()), InteractWidgetClass);
	InventoryWidget->AddToViewport(5);
	InteractWidget->AddToViewport(5);
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	InteractWidget->SetVisibility(ESlateVisibility::Collapsed);
	//GameStart();
}

//틱 함수
void AThreeFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector Velocity = GetVelocity();
	if (Velocity.Size() > 0)
	{
		bShouldMove = true;
	}
	else bShouldMove = false;
	
	if (InventoryWidget && InventoryWidget->IsVisible() == false) InteractCheck();
}

float AThreeFPSCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDive || bIsDead) return 0.f;
	float ActualDamage = DamageAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.f, MaxHealth);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("체력: %f"), CurrentHealth));
	UpdateHP();
	if (HitMontage)
	{
		PlayAnimMontage(HitMontage);
	}
	if (CurrentHealth <= 0.f)
	{
		bIsDead = true;
		Die();
	}
	return ActualDamage;
}

//사망 시 플레이 몽타주 함수
void AThreeFPSCharacter::Die()
{
	bIsDead = true;
	if (HUDInstance)
	{
		HUDInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (UIComponent)
	{
		UIComponent->GameOver();
	}
	if (AThreeFPSPlayerController* PlayerController = Cast<AThreeFPSPlayerController>(GetController()))
	{
		DisableInput(PlayerController);
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
	GetWorldTimerManager().SetTimer(DiedTimer, this, &AThreeFPSCharacter::GameOver, 2.0f, false);
}

void AThreeFPSCharacter::GameOver()
{
	if (GameOverHUDClass)
	{
		if (AThreeFPSPlayerController* PlayerController = Cast<AThreeFPSPlayerController>(GetController()))
		{
			GameOverHUDInstance = CreateWidget<UOnDiedWidget>(PlayerController,GameOverHUDClass);
			GameOverHUDInstance->AddToViewport(6);
			GameOverHUDInstance->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

/* 장착 함수 */
void AThreeFPSCharacter::EquipRifle()
{
	WeaponInventory->EquipWeapon(EGunType::Rifle, this);
	UpdateAmmo();
}

//------------------------//
//        업데이트 함수     //
//-----------------------//

void AThreeFPSCharacter::UpdateStamina()
{
	if (bIsSprinting)
	{
		CurrentStamina = FMath::Clamp(CurrentStamina - StaminaConsumeRate, 0.0f, MaxStamina);
	}
	else CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRegenRate, 0.0f, MaxStamina);
	
	if (CurrentStamina <= 0.0f)
	{
		bIsStaminaEmpty = true;
	}
	else bIsStaminaEmpty = false;

	if (HUDInstance) {
		HUDInstance->SetStaminaBar(CurrentStamina, MaxStamina);
	}
}

void AThreeFPSCharacter::UpdateHP()
{
	if (HUDInstance)
	{
		HUDInstance->SetHealthBar(CurrentHealth, MaxHealth);
	}
}

void AThreeFPSCharacter::UpdateMutation()
{
	CurrentMutation = FMath::Clamp(CurrentMutation+MutationRate, 0.0f, MaxMutation);
	if (HUDInstance)
	{
		HUDInstance->SetMutationBar(CurrentMutation, MaxMutation);
	}
	if (CurrentMutation >= MaxMutation)
	{
		StopMutation();
		Die();
	}
}

//------------------------//
//        입력 함수        //
//-----------------------//
void AThreeFPSCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AThreeFPSCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AThreeFPSCharacter::StartSprint()
{
	if (bIsStaminaEmpty) GetCharacterMovement()->MaxWalkSpeed = OriginSpeed;
	if (bIsAiming || bIsCrouched || !bShouldMove || GetCharacterMovement()->IsFalling()) return;

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	bIsSprinting = true;
}
void AThreeFPSCharacter::StopSprint()
{
	if (bIsAiming || bIsCrouched || bIsStaminaEmpty|| !bShouldMove)
	{
		bIsSprinting = false;
		return;
	}
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = OriginSpeed;
	
}
void AThreeFPSCharacter::StartCrouch()
{
	if (GetCharacterMovement()->IsFalling() || bIsSprinting) return;
	bIsCrouched = true;
	Crouch();
}
void AThreeFPSCharacter::StopCrouch()
{
	if (GetCharacterMovement()->IsFalling() || bIsSprinting) return;
	bIsCrouched = false;
	UnCrouch();
}
void AThreeFPSCharacter::StartAim()
{
	if (InventoryWidget && InventoryWidget->IsVisible()) return;
	if (bIsReloading || bIsSprinting || bIsDive) return;
	GetCharacterMovement()->MaxWalkSpeed = OriginSpeed / 2;
	bIsAiming = true;
	
}
void AThreeFPSCharacter::StopAim()
{
	if (bIsSprinting)
	{
		bIsAiming = false;
		return;
	}
	bIsAiming = false;
	GetCharacterMovement()->MaxWalkSpeed = OriginSpeed;
}

void AThreeFPSCharacter::Jump()
{
	if (!bCanJump || bIsAiming || bIsDive) return;
	Super::Jump();
	bCanJump = false;
	bIsJumping = true;
	GetWorldTimerManager().SetTimer(JumpTimer, this, &AThreeFPSCharacter::ResetJumpTimer, .9f);
}

void AThreeFPSCharacter::ResetJumpTimer()
{
	bCanJump = true;;
	bIsJumping = false;
}

void AThreeFPSCharacter::StartFiring()
{
	if (bIsSprinting || GetCharacterMovement()->IsFalling() || bIsSprinting || bIsDive) return;
	if (WeaponInventory && WeaponInventory->GetCurrentWeapon()->CanFire())
	{
		bIsFiring = true;
		WeaponInventory->GetCurrentWeapon()->StartFire();
	}	
	UpdateAmmo();
}

void AThreeFPSCharacter::StopFiring()
{
	if (WeaponInventory && WeaponInventory->GetCurrentWeapon())
	{
		WeaponInventory->GetCurrentWeapon()->StopFire();
		bIsFiring = false;
	}
}

void AThreeFPSCharacter::StartReload()
{
	if (bIsDive) return;
	if (WeaponInventory && WeaponInventory->GetCurrentWeapon())
	{
		AGunBase* CurrentWeapon = WeaponInventory->GetCurrentWeapon();
		if (!CurrentWeapon->CanReloading() || bIsAiming || bIsFiring || bIsDive) return;
		bIsReloading = true;
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &AThreeFPSCharacter::OnReloaded, CurrentWeapon->GetReloadTime(), false);
		CurrentWeapon->StartReload();
		
	}
}
void AThreeFPSCharacter::OnReloaded()
{
	GetWorldTimerManager().ClearTimer(ReloadTimer);
	UpdateAmmo();
	bIsReloading = false;
	
}

void AThreeFPSCharacter::StartDive()
{
	if (bIsDive || bIsJumping || bIsAiming || CurrentStamina < 30.f ||!bShouldMove) return;
	
	bIsDive = true;
	GetCharacterMovement()->MaxWalkSpeed = OriginSpeed * 2.f;
	CurrentStamina -= 30.f;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("다이브")));
	GetWorldTimerManager().SetTimer(DiveTimer, this, &AThreeFPSCharacter::ResetDiveTimer, 0.94f, false);
}

void AThreeFPSCharacter::ResetDiveTimer()
{
	bIsDive = false;
	if (bIsAiming) return;
	GetCharacterMovement()->MaxWalkSpeed = OriginSpeed;
}

//장전 했을 때 UI업데이트
void AThreeFPSCharacter::UpdateAmmo()
{
	if (HUDInstance)
	{
		HUDInstance->SetCurrentAmmoText(WeaponInventory->GetCurrentWeapon()->GetCurrentAmmo());
		HUDInstance->SetMaxAmmoText(WeaponInventory->GetCurrentWeapon()->GetMaxAmmo());
	}
}

void AThreeFPSCharacter::StopMutation()
{
	GetWorldTimerManager().ClearTimer(UpdateMutationTimer);
}

void AThreeFPSCharacter::InteractCheck()
{
	GetController()->GetPlayerViewPoint(ViewVector, ViewRotation);
	FVector VecDirection = ViewRotation.Vector() * 1000.f;
	InteractVectorEnd = ViewVector + VecDirection;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(InteractHitResult, ViewVector, InteractVectorEnd, ECollisionChannel::ECC_GameTraceChannel2, QueryParams); 
	if (Cast<AItemBase>(InteractHitResult.GetActor()))
	{
		InteractWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InteractWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void AThreeFPSCharacter::Interact()
{
	if (Cast<AItemBase>(InteractHitResult.GetActor()))
	{
		FItemData* Data = ItemDatabase->Items.FindByPredicate([&](const FItemData& ItemData)
			{
				return ItemData.Class == InteractHitResult.GetActor()->GetClass();
			});
		Inventory.Emplace(*Data);
		InteractHitResult.GetActor()->Destroy();
	}
}

void AThreeFPSCharacter::ToggleInventory()
{
	if (IsValid(InventoryWidget))
	{
		if (!InventoryWidget->IsVisible())
		{
			InteractWidget->SetVisibility(ESlateVisibility::Collapsed);

			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			InventoryWidget->RefreshInventory(Inventory);

			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			Cast<APlayerController>(GetController())->SetInputMode(InputMode);
			Cast<APlayerController>(GetController())->SetCinematicMode(true, true, true);
			Cast<APlayerController>(GetController())->bShowMouseCursor = true;
		}
		else
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);

			Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameOnly());
			Cast<APlayerController>(GetController())->SetCinematicMode(false, true, true);
			Cast<APlayerController>(GetController())->bShowMouseCursor = false;
		}
	}
}