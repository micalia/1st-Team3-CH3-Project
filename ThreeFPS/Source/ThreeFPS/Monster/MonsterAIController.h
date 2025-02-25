// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

UENUM(BlueprintType)
enum class EPATROLTYPE :uint8
{
	Empty,
	TargetKey,
	Random
};
UCLASS()
class THREEFPS_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void UpdatePatrolState(EPATROLTYPE type);


	virtual void OnPossess(APawn* pawn) override;
	virtual void BeginPlay() override;


	void TempOnMoveCompleted();

	//virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
protected:
	// 현재 순찰 지점 인덱스
	int32 CurrentPatrolIndex = 0;
	bool bFound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Costume")
	EPATROLTYPE PatrolType = EPATROLTYPE::Empty;

	// 현재 순찰 지점으로 이동하는 공통 함수
	//void MoveToCurrentPatrolPoint();

};
