// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HUDWidget.h"
#include "Components/ProgressBar.h"

void UHUDWidget::SetHealthBar(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}
void UHUDWidget::SetStaminaBar(float CurrentStamina, float MaxStamina)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(CurrentStamina / MaxStamina);
	}
}