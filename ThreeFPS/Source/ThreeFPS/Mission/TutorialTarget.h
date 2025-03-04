// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialTarget.generated.h"

UCLASS()
class THREEFPS_API ATutorialTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	ATutorialTarget();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnHitTarget();

};
