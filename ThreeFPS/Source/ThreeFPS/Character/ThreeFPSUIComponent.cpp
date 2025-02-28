// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ThreeFPSUIComponent.h"
#include "CrosshairWidget.h"
#include "ThreeFPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UThreeFPSUIComponent::UThreeFPSUIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxAimSize = 500;
	MinAimSize = 100;
}

void UThreeFPSUIComponent::BeginPlay()
{
	Super::BeginPlay();
	if (WidgetClass)
	{
		CrosshairWidget = CreateWidget<UCrosshairWidget>(GetWorld(), WidgetClass);
		if (CrosshairWidget)
		{
			CrosshairWidget->AddToViewport();
		}
	}
}

void UThreeFPSUIComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateCrossHair(DeltaTime);
}

void UThreeFPSUIComponent::UpdateCrossHair(float DeltaTime)
{
	if (!CrosshairWidget) return;
	// Example: Update crosshair based on actor's velocity
	AActor* Owner = GetOwner();
	AThreeFPSCharacter* PlayerCharacter = Cast<AThreeFPSCharacter>(Owner);
	if (UCharacterMovementComponent* CharacterMovement = PlayerCharacter->GetCharacterMovement())
	{
		FVector Velocity = PlayerCharacter->GetVelocity();
		float Speed = Velocity.Size();
		bool IsJumping = CharacterMovement->IsFalling();
		
		if (Speed == 0 && AimSize == 0)return; 
		
    
		if (IsJumping) {
			AimSize += 2000.f * DeltaTime;
		}
		else {
			if (Speed > 100)
			{
				AimSize += Speed * DeltaTime; 
			}
			else if (Speed > 0)
			{
				AimSize -= FMath::Clamp(Speed * DeltaTime * 50, 0.0f, AimSize);
			}
			else
			{
				AimSize -= FMath::Clamp(2000.f * DeltaTime, 0.0f, AimSize);
			}
		}
		// 최소 크기 제한
		AimSize = FMath::Clamp(AimSize, MinAimSize, MaxAimSize);
		CrosshairWidget->UpdateCrosshairSize(AimSize);
	}
}