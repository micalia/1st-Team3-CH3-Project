// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThreeFPSGameMode.h"

#include "ThreeFPSPlayerController.h"
#include "ThreeFPS/Character/ThreeFPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThreeFPSGameMode::AThreeFPSGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerFinder(TEXT("/Game/SHyeon/Blueprints/BP_ThreeFPSPlayerController"));
	if (PlayerControllerFinder.Succeeded())
	{
		PlayerControllerClass = PlayerControllerFinder.Class;
	}
}