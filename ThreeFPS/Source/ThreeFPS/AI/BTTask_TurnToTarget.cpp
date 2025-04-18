// Boss AI - 신설빈


#include "AI/BTTask_TurnToTarget.h"
#include "BossAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BossWerewolf.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	if (auto Boss = Cast<ABossWerewolf>(ControllingPawn)) {
		float TurnSpeed = Boss->TurnSpeed;
		FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
		LookVector.Z = 0.0f;
		FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
		ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}