// Boss AI - 신설빈


#include "AI/ShockWavePoint.h"
#include "AI/ShockWaveManager.h"
#include "CableComponent.h"
#include "Character/ThreeFPSCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AShockWavePoint::AShockWavePoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	CableComp = CreateDefaultSubobject<UCableComponent>(TEXT("CableComp"));
	CableComp->SetupAttachment(RootComp);
	SphereCollComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollComp"));
	SphereCollComp->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AShockWavePoint::BeginPlay()
{
	Super::BeginPlay();

	SphereCollComp->OnComponentBeginOverlap.AddDynamic(this, &AShockWavePoint::OnOverlap);
	SphereCollComp->OnComponentEndOverlap.AddDynamic(this, &AShockWavePoint::EndOverlap);
}

// Called every frame
void AShockWavePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShockWavePoint::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto Player = Cast<AThreeFPSCharacter>(OtherActor))
	{
		if (auto WaveManager = Cast<AShockWaveManager>(GetOwner()))
		{
			if (WaveManager->DamageActor == nullptr)
			{
				// TODO: 데미지를 입힌다
				WaveManager->DamageActor = this;
			}
		}
	}
}

void AShockWavePoint::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (auto Player = Cast<AThreeFPSCharacter>(OtherActor))
	{
		if (TObjectPtr<AShockWaveManager> WaveManager = Cast<AShockWaveManager>(GetOwner()))
		{
			if (WaveManager->DamageActor == this) {
				FTimerHandle delay;
				GetWorld()->GetTimerManager().SetTimer(delay, FTimerDelegate::CreateLambda([WaveManager, this, OtherActor]() {
					if (IsValid(WaveManager)) {
						WaveManager->DamageActor = nullptr;
					}
					}), 2.0f, false);
			}
		}
	}
}