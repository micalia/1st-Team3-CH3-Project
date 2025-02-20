// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ThreeFPSGameMode.generated.h"

UCLASS(minimalapi)
class AThreeFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AThreeFPSGameMode();
	
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> IntroHUDWidgetClass;
	UPROPERTY()
	UUserWidget* IntroHUDWidget;
};



