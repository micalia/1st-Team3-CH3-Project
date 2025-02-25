// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWerewolf.h"
#include "Components/CapsuleComponent.h"
#include "BossWerewolfAIController.h"
#include "Character/ThreeFPSCharacter.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	GetMesh()->SetRelativeLocation(FVector(0, 0, -76));

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

     static ConstructorHelpers::FClassFinder<UBossWerewolfAnim> tempAnim(TEXT("/Game/SB/Blueprint/AB_BossWerewolf.AB_BossWerewolf_C"));
    if (tempAnim.Succeeded()) {
        GetMesh()->SetAnimClass(tempAnim.Class);
    }
    GetCapsuleComponent()->SetCapsuleHalfHeight(77);
    GetCapsuleComponent()->SetCapsuleRadius(71);

    static ConstructorHelpers::FClassFinder<ABossWerewolfAIController> tempAiCon(TEXT("/Game/SB/Blueprint/BP_BossWerewolfAIController.BP_BossWerewolfAIController_C"));
    if (tempAiCon.Succeeded()) {
        AIControllerClass = tempAiCon.Class;
    }
    
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    //GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

// Called when the game starts or when spawned
void ABossWerewolf::BeginPlay()
{
	Super::BeginPlay();

 //   DrawBezierCurve();
}

// Called every frame
void ABossWerewolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    CalculateDistance();
    /*if (bIsMoving)
    {
        ElapsedTime += DeltaTime;
        float Alpha = FMath::Clamp(ElapsedTime / 3, 0.0f, 1.0f);

        FVector NewLocation =
            FMath::Lerp(FMath::Lerp(P0, P1, Alpha),
                FMath::Lerp(P1, P2, Alpha),
                Alpha);

        SetActorLocation(NewLocation);

        for (float t = 0.0f; t <= 1.0f; t += 0.05f)
        {
            FVector Point =
                FMath::Lerp(FMath::Lerp(P0, P1, t),
                    FMath::Lerp(P1, P2, t),
                    t);

            DrawDebugSphere(GetWorld(), Point, 5.0f, 8, FColor::Yellow, false, 999.f);
        }

        if (Alpha >= 1.0f)
        {
            bIsMoving = false;
        }
    }*/
}

// Called to bind functionality to input
void ABossWerewolf::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void ABossWerewolf::CalculateDistance()
{
    if (auto Target = GetTarget()) {
        DistanceToTarget = (GetActorLocation() - Target->GetActorLocation()).Length();
        //GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >> Dis: %f"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S")), DistanceToTarget), true, FVector2D(1.5f, 1.5f));
    }
}

TObjectPtr<AThreeFPSCharacter> ABossWerewolf::GetTarget()
{
    if (TargetPtr->IsValidLowLevel()) {
        return TargetPtr;
    }
    else {
        for (TActorIterator<AThreeFPSCharacter> it(GetWorld()); it; ++it) {
            TargetPtr = *it;
        }
        if (TargetPtr->IsValidLowLevel()) {
            return TargetPtr;
        }
    }
    return nullptr;
}