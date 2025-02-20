// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UProgressBar;

UCLASS()
class THREEFPS_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetHealthBar(float CurrentHealth, float MaxHealth);
	void SetStaminaBar(float CurrentStamina, float MaxStamina);

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UProgressBar* StaminaBar;
};
