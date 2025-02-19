// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SBTest.generated.h"

UCLASS()
class THREEFPS_API ASBTest : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASBTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USceneComponent* FootComp;
public:
	UFUNCTION(BlueprintCallable)
	void JumpAttack(class ABossWerewolf* Target);
	//void UpdateJump(float Value);
	//void Attack(class ABossWerewolf* Target);
	float Gravity = 980.0f;
	float Speed = 0;
	bool bIsJumping = false;
	float CurrentJumpTime = 0;
	float JumpDuration = 0;
	float JumpHeight = 0;
	FVector StartLocation = FVector(0);
	FVector EndLocation = FVector(0);
	FVector HorizontalVelocity = FVector(0);
	float InitialVerticalVelocity = 0;

	void CalculateJumpPath(int32 NumPoints);
	UPROPERTY()
	TArray<FVector> JumpPathPoints;
};
