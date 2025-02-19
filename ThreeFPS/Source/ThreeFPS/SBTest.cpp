// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTest.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/TimelineComponent.h"
#include "Components/CapsuleComponent.h"
#include "BossWerewolf.h"

// Sets default values
ASBTest::ASBTest()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    FootComp= CreateDefaultSubobject<USceneComponent>(TEXT("FootComp"));
    FootComp->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ASBTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASBTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsJumping)
	{
		CurrentJumpTime += DeltaTime;

		// 포물선 운동 공식 적용
		FVector NewLocation = StartLocation + (HorizontalVelocity * CurrentJumpTime);
		NewLocation.Z = StartLocation.Z + (InitialVerticalVelocity * CurrentJumpTime) - (0.5f * 980.0f * CurrentJumpTime * CurrentJumpTime);

		SetActorLocation(NewLocation);

		// 목표 지점 도착 여부 확인 (거리 기반 종료)
		float CurrentDistance = FVector::Dist(NewLocation, EndLocation);
		if (CurrentDistance <= 300.f)  // 목표 지점과 매우 가까워지면 종료
		{
			bIsJumping = false;
			GEngine->AddOnScreenDebugMessage(-1, 999, FColor::Purple, FString::Printf(TEXT("%s >>Attack!!"), *FDateTime::UtcNow().ToString(TEXT("%H:%M:%S"))), true, FVector2D(1.5f, 1.5f));
		}
	}
   
}
#pragma optimize("" , off)
// 점프 공격 함수
void ASBTest::JumpAttack(ABossWerewolf* Boss)
{
    //if (!Boss || !JumpMontage) return;

     // 시작 위치와 목표 위치 설정
    StartLocation = FootComp->GetComponentLocation();
    DrawDebugSphere(GetWorld(), StartLocation, 40.0f, 32, FColor::Red, false, 0.1f);
    EndLocation = Boss->FootComp->GetComponentLocation();
    DrawDebugSphere(GetWorld(), EndLocation, 40.0f, 32, FColor::Blue, false, 0.1f);
    
    // 타겟과의 3D 거리 계산
    float DistanceXY = FVector::Dist2D(StartLocation, EndLocation);
    float DeltaHeight = EndLocation.Z - StartLocation.Z;

    //  최적의 점프 지속 시간 계산 (물리 법칙 기반)
    JumpDuration = FMath::Sqrt((2 * FMath::Abs(DeltaHeight) + 2 * DistanceXY) / Gravity);
    JumpDuration = FMath::Clamp(JumpDuration, 0.5f, 2.5f);  // 최소~최대 제한

    //  초기 수직 속도 (높이 차이 반영)
    InitialVerticalVelocity = (Gravity * JumpDuration) / 2 + (DeltaHeight / JumpDuration);

    //  수평 속도 계산
    Speed = DistanceXY / JumpDuration;
    HorizontalVelocity = (EndLocation - StartLocation).GetSafeNormal() * Speed;
    HorizontalVelocity.Z = 0.0f;

    // 점프 공격 활성화
    bIsJumping = true;
    CurrentJumpTime = 0.0f;

    // 궤적 디버그
    CalculateJumpPath(20);
}
#pragma optimize("" , on) 
#pragma optimize("" , off)
void ASBTest::CalculateJumpPath(int32 NumPoints)
{
    JumpPathPoints.Empty();  // 배열 초기화

    float TotalDistance = FVector::Dist(StartLocation, EndLocation);
    float StepDistance = TotalDistance / (NumPoints - 1);

    FVector Direction = (EndLocation - StartLocation).GetSafeNormal();

    for (int i = 0; i < NumPoints; i++)
    {
        FVector PointLocation = StartLocation + Direction * (StepDistance * i);
        float DistanceTraveled = FVector::Dist(StartLocation, PointLocation);

        //  거리 기반 Z 위치 계산 (포물선 공식 적용)
        float TimeAtPoint = DistanceTraveled / HorizontalVelocity.Size();
        PointLocation.Z = StartLocation.Z + (InitialVerticalVelocity * TimeAtPoint) - (0.5f * Gravity * TimeAtPoint * TimeAtPoint);

        JumpPathPoints.Add(PointLocation);

        // 디버그 구체
        DrawDebugSphere(GetWorld(), PointLocation, 10.0f, 12, FColor::Red, false, 5.0f);
    }
}
#pragma optimize("" , on) 