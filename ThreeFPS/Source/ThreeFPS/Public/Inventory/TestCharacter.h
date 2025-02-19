#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestCharacter.generated.h"

class UInputComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class THREEFPS_API ATestCharacter : public ACharacter
{
	GENERATED_BODY()

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
	TSubclassOf<UUserWidget> InteractWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	UUserWidget* InteractWidget;


public:
	ATestCharacter();

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	void Interact();
	void InteractCheck();

	FHitResult InteractHitResult;
	FVector ViewVector;
	FRotator ViewRotation;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
