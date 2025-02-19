#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThreeFPS/Item/ItemDatabase.h"
#include "TestCharacter.generated.h"

class UInputComponent;
class UInputAction;
class UInputMappingContext;
class UInventoryWidget;
class UItemDatabase;
struct FInputActionValue;

UCLASS()
class THREEFPS_API ATestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* JumpAction;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* MoveAction;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* LookAction;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InventoryAction;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> InventoryWidgetClass;
	UPROPERTY()
	UInventoryWidget* InventoryWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> InteractWidgetClass;
	UPROPERTY()
	UUserWidget* InteractWidget;

	UPROPERTY(EditDefaultsOnly)
	UItemDatabase* ItemDatabase;

	TArray<FItemData> Inventory;

	ATestCharacter();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	void Interact();
	void InteractCheck();
	void ToggleInventory();

	FVector ViewVector;
	FRotator ViewRotation;
	FVector InteractVectorEnd;
	FHitResult InteractHitResult;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
