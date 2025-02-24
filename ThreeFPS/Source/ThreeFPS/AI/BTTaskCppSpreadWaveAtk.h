// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskCppSpreadWaveAtk.generated.h"

/**
 * 
 */
UCLASS()
class THREEFPS_API UBTTaskCppSpreadWaveAtk : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskCppSpreadWaveAtk();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
