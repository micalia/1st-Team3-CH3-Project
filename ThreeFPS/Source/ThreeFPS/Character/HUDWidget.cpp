// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HUDWidget.h"
#include "Components/TextBlock.h"
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

void UHUDWidget::SetCurrentAmmoText(int CurrentAmmo)
{
	if (CurrentAmmoText)
	{
		CurrentAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%u"),CurrentAmmo)));
	}
}

void UHUDWidget::SetMaxAmmoText(int MaxAmmo)
{
	if (MaxAmmoText)
	{
		MaxAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%u"),MaxAmmo)));
	}
}
void UHUDWidget::SetMutationBar(float CurrentMutation, float MaxMutation)
{
	if (MutationBar)
	{
		MutationBar->SetPercent(CurrentMutation / MaxMutation);
	}
}