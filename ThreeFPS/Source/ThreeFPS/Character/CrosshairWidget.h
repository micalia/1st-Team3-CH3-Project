// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "CrosshairWidget.generated.h"

UCLASS()
class THREEFPS_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

	//크로스헤어
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* TopCrosshair;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* BottomCrosshair;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* LeftCrosshair;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* RightCrosshair;

public:
	UFUNCTION(BlueprintCallable, Category = "Crosshair")
	void UpdateCrosshairSize(const float Speed);
	void SetCrosshairVisibility(ESlateVisibility Visibility);
};
