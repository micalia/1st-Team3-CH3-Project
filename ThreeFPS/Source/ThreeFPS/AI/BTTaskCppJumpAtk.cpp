// Boss AI - 신설빈

#include "AI/BTTaskCppJumpAtk.h"
#include "AIController.h"
#include "BossAIInterface.h"
#include "BossWerewolf.h"
#include "BossWerewolfAnim.h"

UBTTaskCppJumpAtk::UBTTaskCppJumpAtk()
{
}

EBTNodeResult::Type UBTTaskCppJumpAtk::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	FAIBossAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	if (ABossWerewolf* Boss = Cast<ABossWerewolf>(ControllingPawn)) {
		if (UBossWerewolfAnim* BossAnim = Cast<UBossWerewolfAnim>(Boss->GetMesh()->GetAnimInstance())) {
			BossAnim->SetAIAttackDelegate(OnAttackFinished);
			BossAnim->PlayJumpAtk();
		}
	}

	return EBTNodeResult::InProgress;
}
