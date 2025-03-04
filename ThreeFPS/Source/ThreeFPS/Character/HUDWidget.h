// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class THREEFPS_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetHealthBar(float CurrentHealth, float MaxHealth);
	void SetStaminaBar(float CurrentStamina, float MaxStamina);
	void SetCurrentAmmoText(int CurrentAmmo);
	void SetMaxAmmoText(int MaxAmmo);
	void SetMutationBar(float CurrentMutation, float MaxMutation);
	
	//체력 스태미나
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UProgressBar* StaminaBar;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UProgressBar* MutationBar;
	
	//탄수
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* CurrentAmmoText;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* MaxAmmoText;
};


