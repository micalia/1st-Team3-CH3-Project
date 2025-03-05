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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<ABaseMonster*> MonsterArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 CreateMonsterCountMax;

	FTimerHandle SpawnMonsterTimerHandle;

	UFUNCTION()
	void OnOverlapBeginTriggerBox(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void OnOverlapEndTriggerBox(AActor* OerlappedActor, AActor* OtherActor);

	void SpawnMonsterTimmer();

	void SpawnMonster(int32 idx);
	void DetroryMonster();
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	AMonsterTriggerBox* CurLevelTrigger;
	
public:	
	//virtual void Tick(float DeltaTime) override;

};
