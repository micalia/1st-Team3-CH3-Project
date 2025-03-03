// Boss AI - 신설빈

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShockWaveManager.generated.h"

class AShockWavePoint;

UCLASS()
class THREEFPS_API AShockWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AShockWaveManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetNewLocation(const FVector InNewLoc);

	UFUNCTION(BlueprintImplementableEvent)
	void ConnectCablePointToPoint();
	
	UPROPERTY(EditAnywhere)
	float ShockWaveScale = 0.06f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AShockWavePoint> ShockWavePointFactory;

	UPROPERTY(BlueprintReadWrite)
	TArray<AShockWavePoint*> WavePoints;

	TObjectPtr<AShockWavePoint> DamageActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnCount = 360;

	bool bTargetDamage = false;
};
