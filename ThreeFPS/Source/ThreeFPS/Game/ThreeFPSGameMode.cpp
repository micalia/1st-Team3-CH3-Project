// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThreeFPSGameMode.h"
#include "ThreeFPS/Character/ThreeFPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThreeFPSGameMode::AThreeFPSGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
