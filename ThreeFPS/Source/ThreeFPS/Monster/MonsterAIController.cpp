// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "BaseMonster.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
void AMonsterAIController::OnPossess(APawn* pawn)
{
    Super::OnPossess(pawn);
    bFound = false;
}

void AMonsterAIController::BeginPlay()
{
    Super::BeginPlay();
    //	MoveToCurrentPatrolPoint();
}

/*
void AMonsterAIController::MoveToCurrentPatrolPoint()
{
    ABaseMonster* MyEnemyChar = Cast<ABaseMonster>(GetPawn());
    if (!MyEnemyChar)
        return;

    // 순찰 포인트가 하나도 없다면 이동할 필요 없음
    if (MyEnemyChar->PatrolPoints.Num() == 0)
    {
        return;
    }

    MoveToActor(
        MyEnemyChar->PatrolPoints[CurrentPatrolIndex],
        50.0f,   // AcceptanceRadius: 목표 지점 근처 몇 유닛 이내에 도달하면 멈출지
        true,    // bStopOnOverlap 충돌시 멈추지 않도록 설정
        true,   // bUsePathfinding
        false,  // bCanStrafe: 기본 이동 모드에서 좌우로 회전 없이 이동 가능 여부
        nullptr,// FilterClass: 경로 필터. 디폴트 사용
        true    // bAllowPartialPath: 부분 경로 허용 여부
    );
        //1. AcceptanceRadius : 도착지로부터 해당 범위 내에 들어가면 이동이 종료된다.
        //크게잡는다면 원거리 공격을 할 때의 위치 등으로 쓸 수 있다.
        //2. bStopOnOverlap : 정확한 지점이 아니라 콜리더에 의해 오버랩 되는 지점을 기준으로 이동이 종료된다.
        //3. bUsePathFinding : 체크하면 NavMesh를 사용하여 패스를 찾는다.
        //4. bProjectDestinationToNavigation : 네비게이터 데이터를 사용하기전에 위치를 예측함.
        //5. bCanStrafe : NavMesh를 대각선으로 가로지를 수 있는지 여부
        //6. FilterClass : NavMesh를 이용하여 길을 찾을 때 AreaClass를 고려하여 찾는다.

    //CurrentPatrolIndex = (CurrentPatrolIndex + 1) % MyEnemyChar->PatrolPoints.Num();
}
*/

/*
void AMonsterAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    if (Result.Code == EPathFollowingResult::Success)
    {
        UE_LOG(LogTemp, Warning, TEXT(" 이동 성공!"));
    }
    else if (Result.Code == EPathFollowingResult::Blocked)
    {
        UE_LOG(LogTemp, Error, TEXT(" 이동이 막혔음!"));
    }
    else if (Result.Code == EPathFollowingResult::OffPath)
    {
        UE_LOG(LogTemp, Error, TEXT("AI가 경로를 벗어남!"));
    }
    else if (Result.Code == EPathFollowingResult::Aborted)
    {
        UE_LOG(LogTemp, Error, TEXT("이동이 취소됨!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("이동 실패 (이유 알 수 없음)"));
    }
    return;
    //Success
    //Blocked : Movement was blocked
    //OffPath : Agent is not on path
    //Aborted : Aborted and stopped (failure)
    if (!Result.IsSuccess())
    {
        UE_LOG(LogTemp, Warning, TEXT("OnMoveCompleted Blocked: % d"), Result.Flags);
        UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
        if (NavSys)
        {
            if (NavSys->IsNavigationBeingBuiltOrLocked(GetWorld()))
            {
                UE_LOG(LogTemp, Warning, TEXT("네비게이션이 현재 재생성 중이거나 잠겨 있음!"));


                FVector AI_Location = GetPawn()->GetActorLocation();
                FNavLocation OutNavLocation;

                bool bOnNavMesh = NavSys->ProjectPointToNavigation(AI_Location, OutNavLocation);

                if (bOnNavMesh)
                {
                    UE_LOG(LogTemp, Warning, TEXT("AI는 현재 네비메시 위에 있음!"));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("⚠ AI가 네비메시 밖으로 나감! 길찾기 불가능!"));
                    // 디버그용 네비 메시 영역 시각화
                    DrawDebugSphere(GetWorld(), AI_Location, 30.0f, 12, bOnNavMesh ? FColor::Green : FColor::Red, false, 5.0f);

                    float RandomNavPoint = 1000.0f;
                    FNavLocation RandomLocation;
                    APawn* ControlledPawn = GetPawn();
                    FVector CurrentLocation = ControlledPawn->GetActorLocation();

                    //GetrandomLocationInNavigableRadius
                    bFound = NavSys->GetRandomReachablePointInRadius(CurrentLocation, RandomNavPoint, RandomLocation);

                    if (bFound)
                    {
                        MoveToLocation(RandomLocation.Location);
                        UE_LOG(LogTemp, Warning, TEXT("✅ AI가 랜덤한 네비게이션 가능 위치로 이동 중: %s"), *RandomLocation.Location.ToString());
                        return;
                    }
                    else
                        UE_LOG(LogTemp, Error, TEXT("🚨 랜덤한 네비게이션 위치를 찾을 수 없음!"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("네비게이션이 정상적으로 동작 중"));
            }
        }
    }

    return;

}
*/
void AMonsterAIController::TempOnMoveCompleted()
{
    /*
    if (bFound)
    {
        MoveToCurrentPatrolPoint();
        UE_LOG(LogTemp, Warning, TEXT("bFound true MoveToCurrentPatrolPoint"));
        return;
    }

    if (Result.Code == EPathFollowingResult::Success)
    {
        ABaseMonster* MyEnemyChar = Cast<ABaseMonster>(GetPawn());
        CurrentPatrolIndex = (CurrentPatrolIndex + 1) % MyEnemyChar->PatrolPoints.Num();
        MoveToCurrentPatrolPoint();
    }
    else
    {
        UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
        if (NavSys)
        {
            if (NavSys->IsNavigationBeingBuiltOrLocked(GetWorld()))
            {
                UE_LOG(LogTemp, Warning, TEXT("네비게이션이 현재 재생성 중이거나 잠겨 있음!"));


                FVector AI_Location = GetPawn()->GetActorLocation();
                FNavLocation OutNavLocation;

                bool bOnNavMesh = NavSys->ProjectPointToNavigation(AI_Location, OutNavLocation);

                if (bOnNavMesh)
                {
                    UE_LOG(LogTemp, Warning, TEXT("AI는 현재 네비메시 위에 있음!"));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("⚠ AI가 네비메시 밖으로 나감! 길찾기 불가능!"));
                    // 디버그용 네비 메시 영역 시각화
                    DrawDebugSphere(GetWorld(), AI_Location, 30.0f, 12, bOnNavMesh ? FColor::Green : FColor::Red, false, 5.0f);

                    float RandomNavPoint = 1000.0f;
                    FNavLocation RandomLocation;
                    APawn* ControlledPawn = GetPawn();
                    FVector CurrentLocation = ControlledPawn->GetActorLocation();

                    //GetrandomLocationInNavigableRadius
                    bFound = NavSys->GetRandomReachablePointInRadius(CurrentLocation, RandomNavPoint, RandomLocation );

                    if (bFound)
                    {
                        MoveToLocation(RandomLocation.Location);
                        UE_LOG(LogTemp, Warning, TEXT("✅ AI가 랜덤한 네비게이션 가능 위치로 이동 중: %s"), *RandomLocation.Location.ToString());
                        return;
                    }
                    else
                        UE_LOG(LogTemp, Error, TEXT("🚨 랜덤한 네비게이션 위치를 찾을 수 없음!"));


                    //if (!bFound)
                    //{
                    //    UE_LOG(LogTemp, Warning, TEXT("⚠ 도달 가능한 위치 없음, 탐색 가능한 위치 시도!"));
                    //    bFound = NavSys->GetRandomPointInNavigableRadius(CurrentLocation, RandomNavPoint, RandomLocation);
                    //}
                    //if (bFound)
                    //{
                    //    MoveToLocation(RandomLocation.Location);
                    //    UE_LOG(LogTemp, Warning, TEXT("✅ AI가 랜덤한 네비게이션 가능 위치로 이동 중: %s"), *RandomLocation.Location.ToString());
                    //}
                    //else
                    //    UE_LOG(LogTemp, Error, TEXT("🚨 랜덤한 네비게이션 위치를 찾을 수 없음!"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("네비게이션이 정상적으로 동작 중"));
            }
        }

        //몬스터가 많아지면 네비메시가 길이 막혀 멈춰버릴때가 있다. 실패하면 Waypoint idx를 그대로 다시 시도를 한다.
        FTimerHandle handle;
        GetWorldTimerManager().SetTimer(handle, [this]()
            {
                this->MoveToCurrentPatrolPoint();
                UE_LOG(LogTemp, Warning, TEXT("MoveToCurrentPatrolPoint"));
            },
            2.0f,
            false
            );
    }
    */
}
