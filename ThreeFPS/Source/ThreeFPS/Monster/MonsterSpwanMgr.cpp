

#include "Monster/MonsterSpwanMgr.h"
#include "MonsterTriggerBox.h"
#include "GameFramework/Character.h"
#include "BaseMonster.h"
#include "Zombie.h"
#include "Character/ThreeFPSCharacter.h"

AMonsterSpwanMgr::AMonsterSpwanMgr()
{
	PrimaryActorTick.bCanEverTick = false;
	TempCurlevel = 0;
}

void AMonsterSpwanMgr::OnOverlapBeginTriggerBox(AActor* OverlappedActor, AActor* OtherActor)
{
	if(Cast<AThreeFPSCharacter>(OtherActor)) //if (OtherActor->ActorHasTag("Player"))
	{
		//UE_LOG(LogTemp, Warning, TEXT("OtherActor : %s"), *OtherActor->GetName());
		AMonsterTriggerBox* Trigger = Cast<AMonsterTriggerBox>(OverlappedActor);
		if (Trigger != CurLevelTrigger)
		{
			CurLevelTrigger = Trigger;
			DetroryMonster();//직전 트리거에서 생성되었던 몬스터들을 모두 삭제 한다.(레벨디자인)
			//새로운 트리거에 진입하면 새트리거에 종속되있는 CeateTargetPos들 위치에 몬스터들을 전부 생성시켜놓고 wave가 시작된다.
			int SpawnSizeOfCurTrigger = CurLevelTrigger->Num();
			for (int32 i = 0; i < SpawnSizeOfCurTrigger;++i)
			{
				ABaseMonster*  CreateZombie = CurLevelTrigger->CreateMonsterOfTargetPos(MonsterClass,i);
				MonsterArr.Add(CreateZombie);
			}

			//위에 몬스터들을 전체적으로 깔아놨으니 이제 지정된 초수마다 몬스터들을 한마리씩 생성시킨다=> 게임 벨런스를 유지시키기 위함.
			GetWorld()->GetTimerManager().ClearTimer(SpawnMonsterTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(SpawnMonsterTimerHandle, this, &AMonsterSpwanMgr::SpawnMonsterTimmer, 3.5f, true);
			TempCurlevel++;
			UE_LOG(LogTemp, Warning, TEXT("TempCurlevel = %d"), TempCurlevel);
		}
		//UE_LOG(LogTemp, Warning, TEXT("OtherActor : %s"), *OtherActor->GetName());
	}
}
void AMonsterSpwanMgr::SpawnMonsterTimmer()
{
	ABaseMonster* Monster = CurLevelTrigger->CreateMonster(MonsterClass);
	if (Monster)
		MonsterArr.Add(Monster);

	//CreateMonsterCountMax--;
	//if (0 >= CreateMonsterCountMax)
	//	GetWorld()->GetTimerManager().ClearTimer(SpawnMonsterTimerHandle);
}
void AMonsterSpwanMgr::OnOverlapEndTriggerBox(AActor* OerlappedActor, AActor* OtherActor)
{

}

void AMonsterSpwanMgr::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("MonsterTriggerBoxArr : %d"), MonsterTriggerBoxArr.Num());
	int idx = 0;
	for (AMonsterTriggerBox* Trigger : MonsterTriggerBoxArr)
	{
		Trigger->SetIdx(idx); idx++;
		Trigger->OnActorBeginOverlap.AddDynamic(this, &AMonsterSpwanMgr::OnOverlapBeginTriggerBox);
		Trigger->OnActorEndOverlap.AddDynamic(this, &AMonsterSpwanMgr::OnOverlapEndTriggerBox);
	}

	UClass* LoadedClass = StaticLoadClass(ABaseMonster::StaticClass(), nullptr, TEXT("/Game/KSW/Blueprints/BP_Zombie.BP_Zombie_C"));
	if (LoadedClass)
	{
		MonsterClass = LoadedClass;
		UE_LOG(LogTemp, Warning, TEXT("Monster Class: %s"), *LoadedClass->GetName());
	}
}
void AMonsterSpwanMgr::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (SpawnMonsterTimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(SpawnMonsterTimerHandle);
	DetroryMonster();
}

void AMonsterSpwanMgr::DetroryMonster()
{
	for (int32 i = MonsterArr.Num() - 1; i >= 0; --i)
	{
		ABaseMonster* Monster = MonsterArr[i];
		if (IsValid(Monster) && !Monster->IsActorBeingDestroyed())
			Monster->Destroy();
	}
}


