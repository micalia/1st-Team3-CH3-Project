// Boss AI - Shin Seol Bin

#include "AI/BTTaskCppStoneUpAtk.h"
#include "AIController.h"
#include "BossAIInterface.h"
#include "BossWerewolf.h"
#include "BossWerewolfAnim.h"

UBTTaskCppStoneUpAtk::UBTTaskCppStoneUpAtk()
{
}

EBTNodeResult::Type UBTTaskCppStoneUpAtk::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	if (ABossWerewolf* Boss = Cast<ABossWerewolf>(ControllingPawn)) {
		if (TObjectPtr<UBossWerewolfAnim> BossAnim = Cast<UBossWerewolfAnim>(Boss->GetMesh()->GetAnimInstance())) {
			BossAnim->SetAnimState(EBossWerewolfState::StoneUp);
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
			BossAnim->PlayStoneUpAtk();
		}
	}

	return EBTNodeResult::InProgress;
}
