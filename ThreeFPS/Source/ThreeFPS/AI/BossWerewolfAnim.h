// Boss AI - 신설빈

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAIInterface.h"
#include "BossWerewolfAnim.generated.h"

UENUM(BlueprintType)
enum class EBossWerewolfState :uint8
{
	Idle,
	Move,
	SwitchPhase,
	Die,

	Uppercut,
	JumpAtk,
	StoneUp,
	SpreadWave
};

UCLASS()
class THREEFPS_API UBossWerewolfAnim : public UAnimInstance, public IBossAIInterface
{
	GENERATED_BODY()

public:
	UBossWerewolfAnim();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void SetAnimState(EBossWerewolfState InNewState);
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	EBossWerewolfState GetAnimState();

	UFUNCTION(BlueprintCallable)
	void PlayUppercutAtk();
	UFUNCTION(BlueprintCallable)
	void PlayJumpAtk();
	UFUNCTION(BlueprintCallable)
	void PlayStoneUpAtk();
	UFUNCTION(BlueprintCallable)
	void PlaySpreadWaveAtk();
	UFUNCTION(BlueprintCallable)
	void PlaySwitchPhase();
	UFUNCTION(BlueprintCallable)
	void PlayDie();

	void AttackEnd(UAnimMontage* InMontage, bool bInterrupted);

public:
	UPROPERTY()
	TObjectPtr<UAnimMontage> UppercutMontage;
	UPROPERTY()
	TObjectPtr<UAnimMontage> JumpAtkMontage;
	UPROPERTY()
	TObjectPtr<UAnimMontage> StoneUpMontage;
	UPROPERTY()
	TObjectPtr<UAnimMontage> SpreadWaveMontage;
	UPROPERTY()
	TObjectPtr<UAnimMontage> DieMontage;

	virtual void SetAIAttackDelegate(const FAIBossAttackFinished& InOnAttackFinished) override;

	FAIBossAttackFinished OnAttackFinished;

	float JumpAttackStartTime;	
	float JumpAttackEndTime;	
private:
	EBossWerewolfState CurrAnimState = EBossWerewolfState::Idle;
};
