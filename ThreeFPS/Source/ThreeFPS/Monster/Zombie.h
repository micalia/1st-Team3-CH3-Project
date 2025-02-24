#pragma once

#include "CoreMinimal.h"
#include "BaseMonster.h"
#include "Components/StaticMeshComponent.h"
#include "Zombie.generated.h"

UENUM(BlueprintType)enum class EZONBIE_ST : uint8
{
	DELAY,
	IDLE,
	PATROL,
	CHASE,
	DAMAGE,
	ATTACK,
	DIE
};
UCLASS()
class THREEFPS_API AZombie : public ABaseMonster
{
	GENERATED_BODY()
public:
	AZombie();

	void SetClothingMeshs(USkeletalMesh* Pants, USkeletalMesh* Shirt, USkeletalMesh* Hair, UStaticMesh* HairStatic);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EZONBIE_ST ZombieState;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State")
	bool bPlayerDetected;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	TArray<float> AttackPowerArr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Costume")
	USkeletalMeshComponent* PantsMeshComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Costume")
	USkeletalMeshComponent* ShirtMeshComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Costume")
	USkeletalMeshComponent* HairMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Costume")
	UStaticMeshComponent* HairStaticMeshComp;

	FTimerHandle GameStateHandle;


	virtual void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp,/*자신*/
		AActor* OtherActor,/*충돌한 액터*/
		UPrimitiveComponent* OtherComp,/*충돌한 액터에 달린 충돌 컴포넌트(Collision)*/
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	virtual void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp,/*자신*/
		AActor* OtherActor,/*충돌한 액터*/
		UPrimitiveComponent* OtherComp,/*충돌한 액터에 달린 충돌 컴포넌트(Collision)*/
		int32 OtherBodyIndex) override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void ApplyRagdoll(FVector HitDirection) override;

	void VariousJombie();
};
