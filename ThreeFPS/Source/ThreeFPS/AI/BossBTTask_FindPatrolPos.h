// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BossBTTask_FindPatrolPos.generated.h"

/**
 *
 */
UCLASS()
class THREEFPS_API UBossBTTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBossBTTask_FindPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
