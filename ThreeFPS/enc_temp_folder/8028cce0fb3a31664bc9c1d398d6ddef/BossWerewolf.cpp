// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWerewolf.h"
#include "Components/CapsuleComponent.h"
#include "BossWerewolfAIController.h"

// Sets default values
ABossWerewolf::ABossWerewolf()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSK(TEXT("/Game/SB/Model/Werebear/Mesh/SK_Werebear_red.SK_Werebear_red"));
	if (tempSK.Succeeded()) {
		GetMesh()->SetSkeletalMesh(tempSK.Object);
	}
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -76));

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

     ConstructorHelpers::FClassFinder<UBossWerewolfAnim> tempAnim(TEXT("/Game/SB/Blueprint/AB_BossWerewolf.AB_BossWerewolf_C"));
    if (tempAnim.Succeeded()) {
        GetMesh()->SetAnimClass(tempAnim.Class);
    }
    GetCapsuleComponent()->SetCapsuleHalfHeight(77);
    GetCapsuleComponent()->SetCapsuleRadius(71);

    ConstructorHelpers::FClassFinder<ABossWerewolfAIController> tempAiCon(TEXT("/Game/SB/Blueprint/BP_BossWerewolfAIController.BP_BossWerewolfAIController_C"));
    if (tempAiCon.Succeeded()) {
        AIControllerClass = tempAiCon.Class;
    }

    ConstructorHelpers::FObjectFinder<UAnimMontage> tempUppercutMontage(TEXT("/Game/SB/Animation/BossWearAnim/Use/AM_Uppercut.AM_Uppercut"));
    if (tempUppercutMontage.Succeeded()) {
        UppercutMontage = tempUppercutMontage.Object;
    }
    
}

// Called when the game starts or when spawned
void ABossWerewolf::BeginPlay()
{
	Super::BeginPlay();

    PlayAnimMontage(UppercutMontage, 1);
 //   DrawBezierCurve();
}

// Called every frame
void ABossWerewolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
