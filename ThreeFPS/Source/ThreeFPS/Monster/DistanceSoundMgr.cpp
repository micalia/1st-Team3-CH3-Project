// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/DistanceSoundMgr.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"

ADistanceSoundMgr* ADistanceSoundMgr::GetInstance(UWorld* World)
{
	//if (NULL == Instance)
	//	Instance = world->SpawnActor<ADistanceSoundMgr>();
	//return Instance;
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World"));
		return nullptr;
	}
	for (TActorIterator<ADistanceSoundMgr> It(World); It; ++It)
		return *It; // 이미 ADistanceSoundMgr가 있으면 반환

	// 새로운 ADistanceSoundMgr를 생성
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ADistanceSoundMgr* SoundMgr = World->SpawnActor<ADistanceSoundMgr>(ADistanceSoundMgr::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (SoundMgr)
	{
		// 가비지 방지
		SoundMgr->SetFlags(RF_Standalone | RF_Public);
		SoundMgr->AddToRoot();
	}

	return SoundMgr;
}

// Sets default values
ADistanceSoundMgr::ADistanceSoundMgr()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.2f;

	SetActorHiddenInGame(false);
	SetActorEnableCollision(false);
}

// Called when the game starts or when spawned
void ADistanceSoundMgr::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ADistanceSoundMgr::GetPlayerLocation()
{
	return PlayerLocation;
}

// Called every frame
void ADistanceSoundMgr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
		PlayerLocation = PlayerCharacter->GetActorLocation();
}
void ADistanceSoundMgr::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

   // UE_LOG(LogTemp, Warning, TEXT("SoundMgr::EndPlay called with reason: %d"), (int32)EndPlayReason);

    // 루트에서 안전하게 제거
    if (IsRooted())
    {
        RemoveFromRoot();
    }

    // 타이머 및 리소스 정리
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
    }
}
ADistanceSoundMgr* ADistanceSoundMgr::Instance = nullptr;