

#include "Monster/MonsterSpwanMgr.h"
#include "MonsterTriggerBox.h"
#include "GameFramework/Character.h"
#include "BaseMonster.h"
#include "Zombie.h"
#include "Character/ThreeFPSCharacter.h"

AMonsterSpwanMgr::AMonsterSpwanMgr()
{
	PrimaryActorTick.bCanEverTick = false;
	CreateMonsterCountMax = 20;
}

void AMonsterSpwanMgr::OnOverlapBeginTriggerBox(AActor* OverlappedActor, AActor* OtherActor)
{
	if(Cast<AThreeFPSCharacter>(OtherActor)) //if (OtherActor->ActorHasTag("Player"))
	{
		AMonsterTriggerBox* Trigger = Cast<AMonsterTriggerBox>(OverlappedActor);
		if (Trigger != CurLevelTrigger)
		{
			CurLevelTrigger = Trigger;
			
			GetWorld()->GetTimerManager().ClearTimer(SpawnMonsterTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(SpawnMonsterTimerHandle, this, &AMonsterSpwanMgr::SpawnMonsterTimmer,3.f,true);

		}
		//UE_LOG(LogTemp, Warning, TEXT("OtherActor : %s"), *OtherActor->GetName());
	}
}
void AMonsterSpwanMgr::SpawnMonsterTimmer()
{
	ABaseMonster* Monster = CurLevelTrigger->CreateMonster(MonsterClass);
	if (Monster)
		MonsterArr.Add(Monster);

	CreateMonsterCountMax--;
	if (0 >= CreateMonsterCountMax)
		GetWorld()->GetTimerManager().ClearTimer(SpawnMonsterTimerHandle);

	//GetWorld()->GetTimerManager().SetTimer(
	//	SpawnMonsterTimerHandle, ([this]() {
	//		ABaseMonster* Monster = CurLevelTrigger->CreateMonster();
	//		if (Monster)
	//			MonsterArr.Add(Monster);
	//		}), 5.f, true);
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
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnMonsterTimerHandle);
	}
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


