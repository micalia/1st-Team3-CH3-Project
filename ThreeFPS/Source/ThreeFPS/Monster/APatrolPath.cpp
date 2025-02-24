// Fill out your copyright notice in the Description page of Project Settings.


#include "APatrolPath.h"

// Sets default values
AAPatrolPath::AAPatrolPath()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

AActor* AAPatrolPath::GetWaypoint(int32 Index) const
{
	if (Waypoints.IsValidIndex(Index))
		return Waypoints[Index];

	return nullptr;
}

int32 AAPatrolPath::Num() const
{
	return Waypoints.Num();
}

// Called when the game starts or when spawned
void AAPatrolPath::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAPatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

