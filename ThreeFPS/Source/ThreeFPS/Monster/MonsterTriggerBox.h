// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "Components/BoxComponent.h"
#include "MonsterTriggerBox.generated.h"

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

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
	int32 Index = 0;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:	
	
	bool bSpawned = false;

};