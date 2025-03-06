#pragma once

#include "CoreMinimal.h"
#include "BaseMonster.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/UserDefinedEnum.h" 
#include "EnumsDefinitions.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "EnumsDefinitions.h"
#include "Zombie.generated.h"


UCLASS()
class THREEFPS_API AZombie : public ABaseMonster
{
	GENERATED_BODY()
public:
	AZombie();

	void SetClothingMeshs(USkeletalMesh* Pants, USkeletalMesh* Shirt, USkeletalMesh* Hair, UStaticMesh* HairStatic);
	
	UFUNCTION(BlueprintCallable,Category="Patrol")
	EPATROLTYPE GetterPatrolType()const;

	UFUNCTION(BlueprintCallable, Category = "Patrol")
	void SetPatrolType(EPATROLTYPE Type);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TArray<USoundCue*> Sounds;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	EPATROLTYPE PatrolType;

	// 에디터에서 User Defined Enum을 선택할 수 있도록 설정
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	//UUserDefinedEnum* PatrolTypeEnum;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	bool bAttackTimming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	bool bDebugDetectionSphere = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UParticleSystem* HitImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	float ImpulseStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UMaterialInterface* DecalMaterial;


	FTimerHandle DamageTimerHandler;
	FTimerHandle SoundDistTimerHandler;

	bool bDetectionOverlapping = false;
	bool bEnableDetection = false;
	ACharacter* TargetActor;
	

	virtual void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp,/*자신*/
		AActor* OtherActor,/*충돌한 액터*/
		UPrimitiveComponent* OtherComp,/*충돌한 액터에 달린 충돌 컴포넌트(Collision)*/
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	virtual void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
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


	virtual void ApplyRagdoll(FVector HitDirection) override;
	virtual void Die() override;
	virtual void EnableDetection()override;
	virtual void DisableDetection() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,Category = "State")
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual float Attack() override;
	
	virtual void AttackTimming(int AttType) override;
	
	UFUNCTION(BlueprintCallable, Category = "Costume")
	void VariousJombie();
	
	UFUNCTION(BlueprintCallable, Category = "State")
	void PauseMoveForDamage(float PauseTime, FHitResult HitResult);

	UFUNCTION(BlueprintCallable, Category = "Effect")
	void SpawnDecalAtLocation(FVector Location, FRotator Rotation, float LifeSpan);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlaySound(ESNDZOMBIE type);

	void DistanceSoundVolume();


};
