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
#include "ThreeFPSUIComponent.h"
#include "InputActionValue.h"
#include "Inventory/InventoryWidget.h"
#include "MovieSceneTracksComponentTypes.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "ViewportToolbar/UnrealEdViewportToolbar.h"
#include "Weapon/EGunType.h"
#include "Weapon/Rifle.h"
#include <Item/ItemBase.h>


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
	StaminaConsumeRate = 5.0f;
	StaminaRegenRate = 3.0f;

	//HUD
	HUDClass =nullptr;
	HUDInstance = nullptr;

	//발사 변수
	bIsFiring=false;
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
				EnhancedInputComponent->BindAction(PlayerController->JumpAction,ETriggerEvent::Started,this, &ACharacter::Jump);
				EnhancedInputComponent->BindAction(PlayerController->JumpAction,ETriggerEvent::Completed,this, &ACharacter::StopJumping);
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
			if (PlayerController->EquipRifleAction) EnhancedInputComponent->BindAction(PlayerController->EquipRifleAction, ETriggerEvent::Triggered, this, &AThreeFPSCharacter::EquipPistol);
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
	
	//무기 인벤에 추가
	if (RifleClass)
	{
		if (AGunBase* Rifle = GetWorld()->SpawnActor<AGunBase>(RifleClass))
		{
			WeaponInventory->AddWeapon(EGunType::Rifle, Rifle);
		}
	}
	EquipRifle();
	//스프링암 타임라인
	if (AimCurve)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindUFunction(this, FName("UpdateAimProgress"));
		AimTimeLine.AddInterpFloat(AimCurve, TimeLineProgress);
	}
	//체력 UI업데이트
	UpdateHP();
	
	//스테미너 업데이트 타이머
	GetWorldTimerManager().SetTimer(UpdateStaminaTimer, this, &AThreeFPSCharacter::UpdateStamina, 0.1f, true);
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
	GameStart();
}

//틱 함수
void AThreeFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AimTimeLine.TickTimeline(DeltaTime);
	
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
	float ActualDamage = DamageAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.f, MaxHealth);
	UpdateHP();
	if (CurrentHealth <= 0.f)
	{
		Die();
	}
	return ActualDamage;
}

void AThreeFPSCharacter::Die()
{
}

/* 장착 함수 */
void AThreeFPSCharacter::EquipRifle()
{
	WeaponInventory->EquipWeapon(EGunType::Rifle, this);
}
void AThreeFPSCharacter::EquipPistol()
{
	WeaponInventory->EquipWeapon(EGunType::Pistol,this);
}

//------------------------//
//        업데이트 함수     //
//-----------------------//

void AThreeFPSCharacter::UpdateStamina()
{
	if (bIsSprinting)
	{
		CurrentStamina = FMath::Clamp(CurrentStamina - StaminaConsumeRate, 0.0f, MaxStamina);
		if (CurrentStamina <= 0.0f)
		{
			bIsStaminaEmpty = true;
		}
		else bIsStaminaEmpty = false;
	}
	else CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRegenRate, 0.0f, MaxStamina);

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
	if (bShouldMove && !GetCharacterMovement()->IsFalling() && !bIsFiring && !bIsAiming)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		bIsSprinting = true;
	}
	else GetCharacterMovement()->MaxWalkSpeed = OriginSpeed;
	
}
void AThreeFPSCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = OriginSpeed;
	bIsSprinting = false;
}
void AThreeFPSCharacter::StartCrouch()
{
	Crouch();
}
void AThreeFPSCharacter::StopCrouch()
{
	UnCrouch();
}
void AThreeFPSCharacter::StartAim()
{
	if (InventoryWidget && InventoryWidget->IsVisible()) return;
	if (!bIsReloading)
	{
		GetCharacterMovement()->MaxWalkSpeed = OriginSpeed / 2;
		bIsAiming = true;
		if (AimCurve)
		{
			AimTimeLine.Play();
			UE_LOG(LogTemp, Display, TEXT("Aim Start"));
		}
	}
}
void AThreeFPSCharacter::StopAim()
{
	bIsAiming = false;
	GetCharacterMovement()->MaxWalkSpeed = OriginSpeed;
	if (AimCurve)
	{
		AimTimeLine.Reverse();
	}
}

void AThreeFPSCharacter::UpdateAimProgress(float Value)
{
	if (InventoryWidget && InventoryWidget->IsVisible()) return;
	if (SpringArm)
	{
		UE_LOG(LogTemp, Log, TEXT("UpdateAimProgress: %f"), Value);
		SpringArm->TargetArmLength = FMath::Lerp(OriginSpringArmLength, AimedSpringArmLength, Value);
	}
}

void AThreeFPSCharacter::StartFiring()
{
	if (!bIsSprinting && !GetCharacterMovement()->IsFalling() && !bIsReloading)
	{
		if (WeaponInventory && WeaponInventory->GetCurrentWeapon())
		{
			WeaponInventory->GetCurrentWeapon()->StartFire();
		}
	}
}

void AThreeFPSCharacter::StopFiring()
{
	if (WeaponInventory && WeaponInventory->GetCurrentWeapon())
	{
		WeaponInventory->GetCurrentWeapon()->StopFire();
	}
}

void AThreeFPSCharacter::StartReload()
{
	if (WeaponInventory && WeaponInventory->GetCurrentWeapon())
	{
		AGunBase* CurrentWeapon = WeaponInventory->GetCurrentWeapon();
		if (CurrentWeapon->CanReloading() && !bIsAiming)
		{
			bIsReloading = true;
			CurrentWeapon->StartReload();
			bIsReloading = CurrentWeapon->IsReloading();
			GetWorldTimerManager().SetTimer(ReloadTimer, this, &AThreeFPSCharacter::OnReloaded, CurrentWeapon->GetFireRate(), false);
		}
	}
}
void AThreeFPSCharacter::OnReloaded()
{
	GetWorldTimerManager().ClearTimer(ReloadTimer);
	bIsReloading = false;
}

void AThreeFPSCharacter::InteractCheck()
{
	Cast<APlayerController>(GetController())->GetPlayerViewPoint(ViewVector, ViewRotation);
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