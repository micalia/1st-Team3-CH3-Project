// Boss AI - 신설빈

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossStoneUpObj.generated.h"

class UCapsuleComponent;

UENUM(BlueprintType)
enum class EStoneUpState :uint8
{
	Up,
	Shake,
	Idle,
	Down,
};

UCLASS()
class THREEFPS_API ABossStoneUpObj : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossStoneUpObj();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SMComp;

private:
	UFUNCTION(BlueprintCallable)
	void ActivateTrap();
	void UpdateStoneUp(float DeltaTime);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap", meta = (AllowPrivateAccess = "true"))
	float RiseDuration = 0.6f;
	float CurrentTime = 0.f;

private:
	void UpdateStoneDown(float DeltaTime);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap", meta = (AllowPrivateAccess = "true"))
	float DownDuration = 2.f;
	float DownCurrentTime = 0.f;

private:
	void Shake(float DeltaTime);

	UPROPERTY(EditAnywhere)
	float ShakeTime = 1.5f;
	float CurrShakeTime = 0;
	UPROPERTY(EditAnywhere)
	float ShakeRange = 20.f;
private:
	void Idle(float DeltaTime);

	UPROPERTY(EditAnywhere)
	float IdleTime = 0.7f;
	float CurrIdleTime = 0;

private:
	FVector StartLocation;
	FVector EndLocation;

	EStoneUpState CurrState = EStoneUpState::Up;

	bool bEndShake = false;
};
