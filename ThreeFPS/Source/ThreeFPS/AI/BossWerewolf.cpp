// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWerewolf.h"
#include "Components/CapsuleComponent.h"
#include "AI/BossWerewolfAIController.h"
// Sets default values
ABossWerewolf::ABossWerewolf()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSK(TEXT("/Game/SB/Model/Werebear/Mesh/SK_Werebear_red.SK_Werebear_red"));
	if (tempSK.Succeeded()) {
		GetMesh()->SetSkeletalMesh(tempSK.Object);
	}
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	AIControllerClass = ABossWerewolfAIController::StaticClass();
	// 배치된 NPC나 SpawnActor를 통해서 스폰시킨 NPC 둘다 AI Controller에 의해 통제하도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; 
}

// Called when the game starts or when spawned
void ABossWerewolf::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossWerewolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossWerewolf::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

