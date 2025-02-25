// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossStoneUpObj.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Math/UnrealMathUtility.h"

ABossStoneUpObj::ABossStoneUpObj()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(RootComp);
	SMComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMComp"));
	SMComp->SetupAttachment(CapsuleComp);
}

void ABossStoneUpObj::BeginPlay()
{
	Super::BeginPlay();

	FVector SpawnLoc = GetActorLocation();
	EndLocation = SpawnLoc;
	SpawnLoc.Z -= CapsuleComp->GetScaledCapsuleHalfHeight() * 1.3;
	StartLocation = SpawnLoc;
}

void ABossStoneUpObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrState) {
	case EStoneUpState::Up:
		UpdateStoneUp(DeltaTime);
		break;
	case EStoneUpState::Shake:
		Shake(DeltaTime);
		break;
	case EStoneUpState::Idle:
		Idle(DeltaTime);
		break;
	case EStoneUpState::Down:
		UpdateStoneDown(DeltaTime);
		break;
	}
}

void ABossStoneUpObj::UpdateStoneUp(float DeltaTime)
{
	CurrentTime += DeltaTime;
	const float Alpha = FMath::Clamp(CurrentTime / RiseDuration, 0.f, 1.f);
	const FVector NewLoc = FMath::Lerp(StartLocation, EndLocation, Alpha);

	SetActorLocation(NewLoc);

	if (Alpha >= 1.f)
	{
		CurrState = EStoneUpState::Shake;
	}
}

void ABossStoneUpObj::UpdateStoneDown(float DeltaTime)
{
	DownCurrentTime += DeltaTime;
	const float Alpha = FMath::Clamp(DownCurrentTime / DownDuration, 0.f, 1.f);
	const FVector NewLoc = FMath::Lerp(EndLocation, StartLocation, Alpha);

	SetActorLocation(NewLoc);

	if (Alpha >= 1.f)
	{
		Destroy();
	}
}

void ABossStoneUpObj::ActivateTrap()
{
	CurrState = EStoneUpState::Up;
	CurrentTime = 0.f;
}

void ABossStoneUpObj::Shake(float DeltaTime)
{
	CurrShakeTime+= DeltaTime;

	if (CurrShakeTime < ShakeTime) {
		float RandValX = FMath::RandRange(-ShakeRange, ShakeRange);
		float RandValY = FMath::RandRange(-ShakeRange, ShakeRange);
		float NewLocX = EndLocation.X + RandValX;
		float NewLocY = EndLocation.Y + RandValY;

		FVector NewLoc = FVector(NewLocX, NewLocY, EndLocation.Z);
		SetActorLocation(NewLoc);
	}
	else {
		CurrState = EStoneUpState::Idle;
	}
}

void ABossStoneUpObj::Idle(float DeltaTime)
{
	CurrIdleTime += DeltaTime;

	if (CurrIdleTime > IdleTime) {
		CurrState = EStoneUpState::Down;
	}
}
