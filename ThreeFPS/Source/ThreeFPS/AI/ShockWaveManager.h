// Boss AI - Shin Seol Bin

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
	// Sets default values for this actor's properties
	AShockWaveManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
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
