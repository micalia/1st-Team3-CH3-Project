// Boss AI - 신설빈


#include "AI/BTDecorator_AttackInRange.h"
#include "AI/BossAIInterface.h"
#include "AI/BossWerewolf.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BossAI.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return false;
	}

	if (auto Boss = Cast<ABossWerewolf>(ControllingPawn)) {
		float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
		float AttackRangeWithRadius = Boss->AttackRange;

		bResult = (DistanceToTarget <= AttackRangeWithRadius);
		if (bResult == false) {
			int a = 1;
		}
		else {
			int s = 1;
		}
		return bResult;
	}
	return false;
}
