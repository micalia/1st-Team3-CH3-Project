// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster/MonsterTriggerBox.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "BaseMonster.h"
#include "APatrolPath.h"
#include "Zombie.h"

AMonsterTriggerBox::AMonsterTriggerBox()
{
	//OnActorBeginOverlap.AddDynamic(this, &AMonsterTriggerBox::OnOverlapBegin);
	//OnActorEndOverlap.AddDynamic(this, &AMonsterTriggerBox::OnOverlapEnd);
}

int AMonsterTriggerBox::GetIdx() const
{
	return Index;
}

bool AMonsterTriggerBox::IsSpawned() const
{
	return bSpawned;
}

void AMonsterTriggerBox::SetIdx(int idx)
{
	Index = idx;
}

// Called when the game starts or when spawned
void AMonsterTriggerBox::BeginPlay()
{
	Super::BeginPlay();
}
ABaseMonster* AMonsterTriggerBox::CreateMonster(TSubclassOf<ABaseMonster> MonsterClassType)
{
	int RandIndx = 0;
	if(PatrolPath )
		RandIndx = FMath::RandRange(0, PatrolPath->Num()-1);

	FVector SpwanPos;
	if (PatrolPath && 0 < PatrolPath->Num())
		SpwanPos = PatrolPath->GetWaypoint(RandIndx)->GetActorLocation();
	else
		SpwanPos = GetActorLocation();

	float RandomYaw = FMath::RandRange(0.0f, 360.0f);
	FRotator SpawnRotation = FRotator(0.0f, RandomYaw, 0.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ABaseMonster* SpawnedMonster = GetWorld()->SpawnActor<ABaseMonster>(MonsterClassType,SpwanPos,SpawnRotation, SpawnParams);
	return SpawnedMonster;
}
ABaseMonster* AMonsterTriggerBox::CreateMonsterOfTargetPos(TSubclassOf<ABaseMonster> MonsterClassType,int TargetPosIdx)
{
	FVector SpwanPos;
	if (PatrolPath && 0 < PatrolPath->Num())
		SpwanPos = PatrolPath->GetWaypoint(TargetPosIdx)->GetActorLocation();
	else
		SpwanPos = GetActorLocation();

	float RandomYaw = FMath::RandRange(0.0f, 360.0f);
	FRotator SpawnRotation = FRotator(0.0f, RandomYaw, 0.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ABaseMonster* SpawnedMonster = GetWorld()->SpawnActor<ABaseMonster>(MonsterClassType, SpwanPos, SpawnRotation, SpawnParams);
	Cast<AZombie>(SpawnedMonster)->SetPatrolType(EPATROLTYPE::Random);
	return SpawnedMonster;
}
int32 AMonsterTriggerBox::Num()
{
	if(PatrolPath)
		return  PatrolPath->Num();
	return 0;
}
void AMonsterTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("Spawned: %hs"), bSpawned ? "true" : "false");
	bSpawned = true;
}

void AMonsterTriggerBox::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("EndOverlap: %s"), *GetActorLocation().ToString());
}

