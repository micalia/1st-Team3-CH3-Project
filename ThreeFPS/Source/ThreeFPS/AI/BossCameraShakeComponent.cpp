// Boss AI - 신설빈

#include "AI/BossCameraShakeComponent.h"

UBossCameraShakeComponent::UBossCameraShakeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UBossCameraShakeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UBossCameraShakeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

