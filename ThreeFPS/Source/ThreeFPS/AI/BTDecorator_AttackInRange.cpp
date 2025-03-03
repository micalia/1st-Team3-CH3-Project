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
#pragma optimize("" , off) 
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
		GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> Dist2Target: %f / AttackRange: %f"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S")), DistanceToTarget, AttackRangeWithRadius), true, FVector2D(1.5f, 1.5f));
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
#pragma optimize("" , on) 