// Boss AI - ½Å¼³ºó

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossCameraShakeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THREEFPS_API UBossCameraShakeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBossCameraShakeComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite)
	bool bEndCamShake = false;
};
