// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWerewolf.h"
#include "Components/CapsuleComponent.h"

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

    if (bIsMoving)
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
    }
}

// Called to bind functionality to input
void ABossWerewolf::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}
