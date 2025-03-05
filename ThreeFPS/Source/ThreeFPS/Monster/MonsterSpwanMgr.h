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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<ABaseMonster*> MonsterArr;



	FTimerHandle SpawnMonsterTimerHandle;

	UFUNCTION()
	void OnOverlapBeginTriggerBox(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void OnOverlapEndTriggerBox(AActor* OerlappedActor, AActor* OtherActor);

	void SpawnMonsterTimmer();

	void DetroryMonster();
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	AMonsterTriggerBox* CurLevelTrigger;
	int TempCurlevel;
public:	
	//virtual void Tick(float DeltaTime) override;

};
