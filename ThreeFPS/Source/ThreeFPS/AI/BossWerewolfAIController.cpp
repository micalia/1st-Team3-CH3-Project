// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWerewolfAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BossAI.h"
#include "BossWerewolf.h"

ABossWerewolfAIController::ABossWerewolfAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/SB/AI/BB_BossWerewolf.BB_BossWerewolf"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/SB/AI/BT_BossWerewolf.BT_BossWerewolf"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void ABossWerewolfAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr)) {
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void ABossWerewolfAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent) {
		BTComponent->StopTree();
	}
}

void ABossWerewolfAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ABossWerewolf* Wolf = Cast<ABossWerewolf>(InPawn)) {
		BossAnim = Cast<UBossWerewolfAnim>(Wolf->GetMesh()->GetAnimInstance());
	}
	RunAI();
}

void ABossWerewolfAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}