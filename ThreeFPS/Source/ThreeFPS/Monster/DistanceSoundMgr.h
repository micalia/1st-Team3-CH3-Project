// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DistanceSoundMgr.generated.h"

UCLASS()
class THREEFPS_API ADistanceSoundMgr : public AActor
{
	GENERATED_BODY()

private:
	static ADistanceSoundMgr* Instance;

	FVector PlayerLocation;

public:	
	static ADistanceSoundMgr* GetInstance(UWorld* World);

	ADistanceSoundMgr();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	FVector GetPlayerLocation();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason);

};
