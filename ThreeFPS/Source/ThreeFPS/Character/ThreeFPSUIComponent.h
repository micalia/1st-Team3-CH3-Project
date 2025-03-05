// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrosshairWidget.h"
#include "Extensions/UIComponent.h"
#include "Blueprint/UserWidget.h"
#include "ThreeFPSUIComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class THREEFPS_API UThreeFPSUIComponent : public UActorComponent
{
	GENERATED_BODY()
	
	float AimSize;
	float MaxAimSize;
	float MinAimSize;
	
	void UpdateCrossHair(float DeltaTime);
	
public:
	UThreeFPSUIComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UCrosshairWidget> WidgetClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	UCrosshairWidget* CrosshairWidget;

	void GameStart();
	void GameOver();
};
