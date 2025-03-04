

#include "Monster/MonsterSpwanMgr.h"
#include "MonsterTriggerBox.h"
#include "GameFramework/Character.h"
#include "BaseMonster.h"
//#include "ThreeFPSCharacter.h"

AMonsterSpwanMgr::AMonsterSpwanMgr()
{
	PrimaryActorTick.bCanEverTick = false;

	UClass* LoadedClass = StaticLoadClass(ABaseMonster::StaticClass(), nullptr, TEXT("/Game/KSW/Blueprints/BP_Zombie.BP_Zombie_C"));
	if (LoadedClass)
	{
		MonsterClass = LoadedClass;
		UE_LOG(LogTemp, Warning, TEXT("Monster Class: %s"), *LoadedClass->GetName());
	}
	
}

void AMonsterSpwanMgr::OnOverlapBeginTriggerBox(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		AMonsterTriggerBox* Trigger = Cast<AMonsterTriggerBox>(OverlappedActor);
		if (Trigger)
		{
			int32 idx = Trigger->GetIdx();
			int32 SpwanPosIdx = idx >= MonsterTriggerBoxArr.Num() - 1 ? MonsterTriggerBoxArr.Num() - 1 : idx + 1;
			
			SpawnMonster(SpwanPosIdx);
		}

		UE_LOG(LogTemp, Warning, TEXT("OtherActor : %s"), *OtherActor->GetName());
	}
}

void AMonsterSpwanMgr::OnOverlapEndTriggerBox(AActor* OerlappedActor, AActor* OtherActor)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("OtherActor : %s"), *Character->GetName());
	}
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

}
void AMonsterSpwanMgr::SpawnMonster(int32 idx)
{
	if (MonsterClass)
	{
		FVector SpwanPos = MonsterTriggerBoxArr[idx]->GetActorLocation();
		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ABaseMonster* SpawnedMonster = GetWorld()->SpawnActor<ABaseMonster>(MonsterClass,SpwanPos,SpawnRotation, SpawnParams);

		if (SpawnedMonster)
		{
			UE_LOG(LogTemp, Warning, TEXT("Monster spawned: %s"), *SpawnedMonster->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed  monster."));
		}
	}
}

//void AMonsterSpwanMgr::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

