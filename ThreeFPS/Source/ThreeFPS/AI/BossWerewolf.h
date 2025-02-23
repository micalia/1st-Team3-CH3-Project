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
	// Sets default values for this character's properties
	ABossWerewolf();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    void CalculateDistance();
	TObjectPtr<AThreeFPSCharacter> GetTarget(); 
    
public:
    UAnimMontage* UppercutMontage;

public:
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
