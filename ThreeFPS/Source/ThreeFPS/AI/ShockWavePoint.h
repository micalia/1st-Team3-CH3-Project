// Boss AI - Shin Seol Bin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShockWavePoint.generated.h"

class UCableComponent;
class USphereComponent;

UCLASS()
class THREEFPS_API AShockWavePoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShockWavePoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCableComponent* CableComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereCollComp;

};
