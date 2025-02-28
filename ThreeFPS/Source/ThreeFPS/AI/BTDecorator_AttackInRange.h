// Boss AI - Shin Seol Bin

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AttackInRange.generated.h"

/**
 * 
 */
UCLASS()
class THREEFPS_API UBTDecorator_AttackInRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_AttackInRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
