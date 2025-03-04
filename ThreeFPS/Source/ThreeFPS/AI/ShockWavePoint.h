// Boss AI - 신설빈

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
	AShockWavePoint();

protected:
	virtual void BeginPlay() override;

public:
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
