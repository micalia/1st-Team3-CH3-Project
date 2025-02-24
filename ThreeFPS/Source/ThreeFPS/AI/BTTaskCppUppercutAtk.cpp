// Fill out your copyright notice in the Description page of Project Settings.

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
			BossAnim->PlayUppercutAtk();
		}
	}
	
    return EBTNodeResult::InProgress;
}
