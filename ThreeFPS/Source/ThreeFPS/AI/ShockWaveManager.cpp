// 보스 AI 담당 : 신설빈


#include "AI/ShockWaveManager.h"
#include "ShockWavePoint.h"

// Sets default values
AShockWaveManager::AShockWaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*static ConstructorHelpers::FClassFinder<AShockWavePoint> tempWavePoint(TEXT("/Game/SB/Blueprint/Attack/BP_WavePoint.BP_WavePoint_C"));
	if (tempWavePoint.Succeeded()) {
		ShockWavePointFactory = tempWavePoint.Class;
	}*/
}

// Called when the game starts or when spawned
void AShockWaveManager::BeginPlay()
{
	Super::BeginPlay();

	FVector SpawnLoc = GetActorLocation();
	FRotator SpawnRot = GetActorRotation();
	SpawnLoc.Z -= 9999;
	
	FActorSpawnParameters Param;
	Param.Owner = this;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (int i = 0; i < SpawnCount; i++)
	{
		SpawnRot.Yaw = i;
		AShockWavePoint* WavePoint = GetWorld()->SpawnActor<AShockWavePoint>(ShockWavePointFactory, SpawnLoc, SpawnRot, Param);
		WavePoints.Add(WavePoint);
	}
	ConnectCablePointToPoint();
}

// Called every frame
void AShockWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShockWaveManager::SetNewLocation(const FVector InNewLoc)
{
	FRotator SpawnRot = GetActorRotation();
	FTransform NewTransform;
	FQuat MyQuat;
	NewTransform.SetLocation(InNewLoc);
	for (int i = 0; i < WavePoints.Num(); i++)
	{
		SpawnRot.Yaw = i;
		MyQuat = SpawnRot.Quaternion();
		NewTransform.SetRotation(MyQuat);
		WavePoints[i]->SetActorTransform(NewTransform);
	}
}

