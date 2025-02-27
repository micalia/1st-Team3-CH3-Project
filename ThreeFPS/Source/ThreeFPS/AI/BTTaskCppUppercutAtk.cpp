// 보스 AI 담당 : 신설빈

#include "AI/BTTaskCppUppercutAtk.h"
#include "AIController.h"
#include "AI/BossWerewolfAnim.h"
#include "AI/BossWerewolf.h"
#include "BossAIInterface.h"

UBTTaskCppUppercutAtk::UBTTaskCppUppercutAtk()
{
}

EBTNodeResult::Type UBTTaskCppUppercutAtk::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	if (ABossWerewolf* Boss = Cast<ABossWerewolf>(ControllingPawn)) {
		if (TObjectPtr<UBossWerewolfAnim> BossAnim = Cast<UBossWerewolfAnim>(Boss->GetMesh()->GetAnimInstance())) {
			BossAnim->SetAnimState(EBossWerewolfState::Uppercut);
			FAIBossAttackFinished OnAttackFinished;
			OnAttackFinished.BindLambda(
				[&, BossAnim]()
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					if (IsValid(BossAnim)) {
						BossAnim->SetAnimState(EBossWerewolfState::Move);
					}
				}
			);

			BossAnim->SetAIAttackDelegate(OnAttackFinished);
			BossAnim->PlayUppercutAtk();
		}
	}

	return EBTNodeResult::InProgress;
}
