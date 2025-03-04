// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster/MonsterTriggerBox.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"

// Sets default values
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

void AMonsterTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Spawned: %hs"), bSpawned ? "true" : "false");
	bSpawned = true;
}

void AMonsterTriggerBox::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("EndOverlap: %s"), *GetActorLocation().ToString());
}

