// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "Components/BoxComponent.h"
#include "APatrolPath.h"
#include "MonsterTriggerBox.generated.h"

class ABaseMonster;
UCLASS()
class THREEFPS_API AMonsterTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterTriggerBox();

	int GetIdx()const;
	bool IsSpawned()const;

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void SetIdx(int idx);

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	ABaseMonster* CreateMonster(TSubclassOf<ABaseMonster> MonsterClassType);

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	ABaseMonster* CreateMonsterOfTargetPos(TSubclassOf<ABaseMonster> MonsterClassType, int TargetPosIdx);

	UFUNCTION(BlueprintPure, Category = "Trigger")
	int32 Num();

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
	AAPatrolPath* PatrolPath;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trigger")
	int32 Index = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trigger||Spawning")
	TArray<ABaseMonster*> MonsterArr;

	
	virtual void BeginPlay() override;



private:	
	
	bool bSpawned = false;

};