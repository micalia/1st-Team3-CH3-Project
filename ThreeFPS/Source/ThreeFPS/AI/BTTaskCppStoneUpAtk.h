
// 보스 AI 담당 : 신설빈

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskCppStoneUpAtk.generated.h"

/**
 * 
 */
UCLASS()
class THREEFPS_API UBTTaskCppStoneUpAtk : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskCppStoneUpAtk();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
