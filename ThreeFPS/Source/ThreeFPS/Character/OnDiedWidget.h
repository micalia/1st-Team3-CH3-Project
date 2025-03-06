// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnDiedWidget.generated.h"

class UButton;
class UWidgetAnimation;

UCLASS()
class THREEFPS_API UOnDiedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// virtual bool Initialize() override;	

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;
	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
};
