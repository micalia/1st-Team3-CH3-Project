// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossWerewolfAnim.generated.h"

UENUM(BlueprintType)
enum class EBossWerewolfState :uint8
{
	Idle,
	Move,
	Die,

	Uppercut,
	JumpAtk,
	StoneUp,
	SpreadCircle
};

UCLASS()
class THREEFPS_API UBossWerewolfAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetAnimState(EBossWerewolfState InNewState){ CurrAnimState = InNewState; };
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	FORCEINLINE EBossWerewolfState GetAnimState(){ return CurrAnimState; };

private:
	EBossWerewolfState CurrAnimState = EBossWerewolfState::Idle;
};
