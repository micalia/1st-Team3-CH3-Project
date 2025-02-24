// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskCppUppercutAtk.generated.h"



UCLASS()
class THREEFPS_API UBTTaskCppUppercutAtk : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskCppUppercutAtk();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
