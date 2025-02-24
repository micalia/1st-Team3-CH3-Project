// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APatrolPath.generated.h"

UCLASS()
class THREEFPS_API AAPatrolPath : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAPatrolPath();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Patrol")
	TArray<AActor*> Waypoints;

	AActor* GetWaypoint(int32 Index) const;
	int32 Num() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
