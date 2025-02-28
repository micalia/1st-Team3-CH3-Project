#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterInterface.h"
#include "BaseMonster.generated.h"

class USphereComponent;
class AAPatrolPath;

UCLASS()
class THREEFPS_API ABaseMonster : public ACharacter, public IMonsterInterface
{
	GENERATED_BODY()

public:
	ABaseMonster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	AAPatrolPath* PatrolPath;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	FName MonsterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Effects")
	UParticleSystem* ParticleBlood;



	virtual void OnCapsuleOverlap(
		UPrimitiveComponent* OverlappedComp,/*자신*/
		AActor* OtherActor,/*충돌한 액터*/
		UPrimitiveComponent* OtherComp,/*충돌한 액터에 달린 충돌 컴포넌트(Collision)*/
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	virtual void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp,/*자신*/
		AActor* OtherActor,/*충돌한 액터*/
		UPrimitiveComponent* OtherComp,/*충돌한 액터에 달린 충돌 컴포넌트(Collision)*/
		int32 OtherBodyIndex) override;

	virtual void OnDetectionOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	virtual void OnDetectionEndOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;

	UFUNCTION(BlueprintCallable, Category = "DebugDetection")
	virtual void EnableDetection()override;
	UFUNCTION(BlueprintCallable, Category = "DebugDetection")
	virtual void DisableDetection() override;
	UFUNCTION(BlueprintCallable, Category = "State")
	virtual float Attack();
	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void AttackTimming(int AttType);
	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void Die() ;

	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void ApplyRagdoll(FVector HitDirection) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	USphereComponent* DetectionSphere;

	int32 Hp;
	float Power = 1.f;
	float Speed;
};
