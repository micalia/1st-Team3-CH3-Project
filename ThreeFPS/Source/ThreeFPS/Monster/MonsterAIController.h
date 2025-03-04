// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnumsDefinitions.h"
#include "MonsterAIController.generated.h"


UCLASS()
class THREEFPS_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "State")
	void UpdatePatrolState2(EPATROLTYPE type);

	virtual void OnPossess(APawn* pawn) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable,Category = "State")
	void RandomSelectPatrolState();

	UFUNCTION(BlueprintCallable, Category = "State")
	void ChaseAfterDamage();

	UFUNCTION(BlueprintCallable, Category = "State")
	void ChargingState(float SightRadius = 1000000.f, float LoseSightRadius = 1000000.f,float PeripheralVisionAngleDegrees = 360.f);

	void TempOnMoveCompleted();

	//virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
protected:
	// 현재 순찰 지점 인덱스
	int32 CurrentPatrolIndex = 0;
	bool bFound;

	UPROPERTY( BlueprintReadWrite, Category = "Costume")
	bool bPlayerDetected; 

	UPROPERTY( BlueprintReadWrite, Category = "Costume")
	EPATROLTYPE PatrolType = EPATROLTYPE::Empty;

	// 현재 순찰 지점으로 이동하는 공통 함수
	//void MoveToCurrentPatrolPoint();

};
