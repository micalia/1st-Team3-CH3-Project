// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossWerewolfAnim.h"
#include "BossWerewolfAIController.generated.h"

class UBlackboardData;
class UBehaviorTree;
class UBossWerewolfAnim;
class AThreeFPSCharacter;

UCLASS()
class THREEFPS_API ABossWerewolfAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABossWerewolfAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UBossWerewolfAnim> BossAnim;
private:
	UPROPERTY()
	TObjectPtr<UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<UBehaviorTree> BTAsset;

};