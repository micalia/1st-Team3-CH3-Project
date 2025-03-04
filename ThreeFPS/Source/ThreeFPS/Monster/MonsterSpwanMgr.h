// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpwanMgr.generated.h"

class AMonsterTriggerBox;
class ABaseMonster;
UCLASS()
class THREEFPS_API AMonsterSpwanMgr : public AActor
{
	GENERATED_BODY()
	
public:	
	AMonsterSpwanMgr();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerBox");
	TArray< AMonsterTriggerBox*> MonsterTriggerBoxArr;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<ABaseMonster> MonsterClass;

	UFUNCTION()
	void OnOverlapBeginTriggerBox(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void OnOverlapEndTriggerBox(AActor* OerlappedActor, AActor* OtherActor);

	void SpawnMonster(int32 idx);
protected:
	virtual void BeginPlay() override;

public:	
	//virtual void Tick(float DeltaTime) override;

};
