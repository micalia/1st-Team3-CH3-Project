#include "TestCharacter.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ThreeFPS/Item/ItemBase.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.h"
#include "ThreeFPS/Item/ItemDatabase.h"

ATestCharacter::ATestCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();

	InventoryWidget = CreateWidget<UInventoryWidget>(Cast<APlayerController>(GetController()), InventoryWidgetClass);
	InteractWidget = CreateWidget(Cast<APlayerController>(GetController()), InteractWidgetClass);
	InventoryWidget->AddToViewport(0);
	InteractWidget->AddToViewport(0);
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	InteractWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InteractCheck();
}

void ATestCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATestCharacter::Move);
		}
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATestCharacter::Look);
		}
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}
		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ATestCharacter::Interact);
		}
		if (InventoryAction)
		{
			EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ATestCharacter::ToggleInventory);
		}
	}
}

void ATestCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATestCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATestCharacter::InteractCheck()
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

void ATestCharacter::Interact()
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

void ATestCharacter::ToggleInventory()
{
	if (!InventoryWidget->IsVisible())
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		InventoryWidget->RefreshInventory(Inventory);

		Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameAndUI());
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