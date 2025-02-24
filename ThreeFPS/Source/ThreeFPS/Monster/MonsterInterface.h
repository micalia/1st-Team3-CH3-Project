// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MonsterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMonsterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class THREEFPS_API IMonsterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	UFUNCTION()
	virtual void OnCapsuleOverlap(
		UPrimitiveComponent* OverlappedComp,/*자신*/
		AActor* OtherActor,/*충돌한 액터*/
		UPrimitiveComponent* OtherComp,/*충돌한 액터에 달린 충돌 컴포넌트(Collision)*/
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;

	UFUNCTION()
	virtual void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp,/*자신*/
		AActor* OtherActor,/*충돌한 액터*/
		UPrimitiveComponent* OtherComp,/*충돌한 액터에 달린 충돌 컴포넌트(Collision)*/
		int32 OtherBodyIndex) = 0;

	UFUNCTION()
	virtual void ApplyRagdoll(FVector HitDirection) = 0;

	int Hp;
	float Speed;

};
