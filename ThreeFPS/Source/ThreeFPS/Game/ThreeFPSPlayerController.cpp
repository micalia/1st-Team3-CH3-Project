// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ThreeFPSPlayerController.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"

AThreeFPSPlayerController::AThreeFPSPlayerController() : MappingContext(nullptr), MoveAction(nullptr), LookAction(nullptr), CrouchAction(nullptr),
														JumpAction(nullptr),FireAction(nullptr),SprintAction(nullptr)	{}


void AThreeFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (MappingContext)
			{
				SubSystem->AddMappingContext(MappingContext,0);
			}
		}
	}
}