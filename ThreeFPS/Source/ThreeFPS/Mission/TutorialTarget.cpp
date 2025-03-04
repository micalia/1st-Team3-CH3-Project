// Fill out your copyright notice in the Description page of Project Settings.


#include "Mission/TutorialTarget.h"

// Sets default values
ATutorialTarget::ATutorialTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATutorialTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATutorialTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

