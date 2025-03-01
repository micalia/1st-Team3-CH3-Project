// Boss AI - 신설빈

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossWerewolf.generated.h"

class UAnimMontage;
class AThreeFPSCharacter;

UCLASS()
class THREEFPS_API ABossWerewolf : public ACharacter
{
	GENERATED_BODY()

public:
	ABossWerewolf();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
    void CalculateDistance();
	TObjectPtr<AThreeFPSCharacter> GetTarget(); 

	void Init();
	void Die();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void Damaged(float InHitDamage) {
		CurrHp -= InHitDamage;
		if (CurrHp < 0) {
			CurrHp = 0;
		}
	}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHp() const { return CurrHp; };

public:
	FVector CalculateBezier(float ratio, const FVector P0, const FVector P1, const FVector P2);
	void JumpAttackPath(const FVector InStartPos, const FVector InBetweenPos, const FVector InEndPos);
	UFUNCTION(BlueprintCallable)
	void MakeJumpAttackTrajectory();
	void JumpAttackState();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = JumpAttack)
	float JumpAttackCurrentTime = 0;
	UPROPERTY(EditAnywhere, Category = JumpAttack)
	float JumpMovingTime = 0.85f;

	UFUNCTION(BlueprintCallable)
	void SetNewGoalDirection();
	void LookAtPlayerToAttack();

	UPROPERTY(EditAnywhere, Category = Bezier)
	float EndPosSub = -40;
	UPROPERTY(EditAnywhere, Category = Bezier)
	float BetweenHeight = 4000;
	UPROPERTY(EditAnywhere, Category = Bezier)
	float CurvePointCount = 7;

	UPROPERTY(EditAnywhere)
	float FullHp = 1000;
private:
	float CurrHp = 0;
	FVector PlayerPosition;
	float TurnAngle;
	FVector ToPlayerDir;
	float PrevTurnAngle;
	float NextTurnAngle;
	UPROPERTY(EditAnywhere)
	float RotationTime = 0.3f;
	float CurrentRotationTime;
	bool bTurnComplete = false;

	FVector StartPos;
	FVector OriginEndPos;
	FVector EndPos;
	TArray<FVector> LineLoc;

	float PtoPCurrTime = 0;
	int32 jumpAttackIdx = 0;
	bool jumpAttackOn = false;

public:
	UPROPERTY(EditAnywhere)
	float AttackRange = 1500;
	UPROPERTY(EditAnywhere)
	float TurnSpeed = 10.0f;

	UPROPERTY(BlueprintReadOnly)
	float DistanceToTarget = 0;

private:
	UPROPERTY()
	TObjectPtr<AThreeFPSCharacter> TargetPtr;

};
