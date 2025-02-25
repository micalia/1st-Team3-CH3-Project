// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossWerewolf.generated.h"

class UAnimMontage;
class AThreeFPSCharacter;

UCLASS()
class THREEFPS_API ABossWerewolf : public ACharacter
{
	GENERATED_BODY()

public:
	ABossWerewolf();

protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void CalculateDistance();
	TObjectPtr<AThreeFPSCharacter> GetTarget(); 

public:
    UPROPERTY(EditAnywhere)
	float AttackRange = 1500;
	UPROPERTY(EditAnywhere)
	float TurnSpeed = 2.0f;

    float ElapsedTime = 0;
    
    UPROPERTY(EditAnywhere, Category = "Bezier")
    FVector P0 = FVector(0, 0, 0);

    UPROPERTY(EditAnywhere, Category = "Bezier")
    FVector P1 = FVector(100, 200, 50);

    UPROPERTY(EditAnywhere, Category = "Bezier")
    FVector P2 = FVector(200, 0, 100);

    
    UPROPERTY(EditAnywhere, Category = "Bezier")
    int32 NumSegments = 20;

    bool bIsMoving = true;

    UPROPERTY(BlueprintReadOnly)
	float DistanceToTarget = 0;

private:
	UPROPERTY()
	TObjectPtr<AThreeFPSCharacter> TargetPtr;

};
