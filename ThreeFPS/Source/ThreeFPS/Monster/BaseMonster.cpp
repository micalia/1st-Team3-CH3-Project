// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMonster.h"
#include "APatrolPath.h"

// Sets default values
ABaseMonster::ABaseMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseMonster::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABaseMonster::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
void ABaseMonster::OnDetectionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABaseMonster::OnDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABaseMonster::EnableDetection()
{
	
}

void ABaseMonster::DisableDetection()
{
}

float ABaseMonster::Attack()
{
	return 0.f;
}

void ABaseMonster::AttackTimming(int AttType)
{
}

void ABaseMonster::ApplyRagdoll(FVector HitDirection)
{
}

void ABaseMonster::Die()
{
}



