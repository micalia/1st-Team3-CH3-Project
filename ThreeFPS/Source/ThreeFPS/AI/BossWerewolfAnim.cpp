// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossWerewolfAnim.h"
#include "AI/BossWerewolf.h"

UBossWerewolfAnim::UBossWerewolfAnim()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> tempUppercutMontage(TEXT("/Script/Engine.AnimMontage'/Game/SB/Animation/BossWearAnim/Use/AM_Uppercut.AM_Uppercut'"));
	if (tempUppercutMontage.Succeeded()) {
		UppercutMontage = tempUppercutMontage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> tempJumpAtkMontage(TEXT("/Script/Engine.AnimMontage'/Game/SB/Animation/BossWearAnim/Use/AM_JumpAtk.AM_JumpAtk'"));
	if (tempJumpAtkMontage.Succeeded()) {
		JumpAtkMontage = tempJumpAtkMontage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> tempStoneUpMontage(TEXT("/Script/Engine.AnimMontage'/Game/SB/Animation/BossWearAnim/Use/AM_StoneUp.AM_StoneUp'"));
	if (tempStoneUpMontage.Succeeded()) {
		StoneUpMontage = tempStoneUpMontage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> tempSpreadWaveMontage(TEXT("/Script/Engine.AnimMontage'/Game/SB/Animation/BossWearAnim/Use/AM_SpreadWave.AM_SpreadWave'"));
	if (tempSpreadWaveMontage.Succeeded()) {
		SpreadWaveMontage = tempSpreadWaveMontage.Object;
	}
}

void UBossWerewolfAnim::SetAnimState(EBossWerewolfState InNewState)
{
	CurrAnimState = InNewState;
}

EBossWerewolfState UBossWerewolfAnim::GetAnimState()
{
	return CurrAnimState;
}

void UBossWerewolfAnim::PlayUppercutAtk()
{
	if (AActor* Owner = TryGetPawnOwner()) {
		if (ABossWerewolf* Boss = Cast<ABossWerewolf>(Owner)) {
			if (UppercutMontage) {
				Montage_Play(UppercutMontage);
				FOnMontageEnded MontageEndedDel;
				MontageEndedDel.BindUObject(this, &UBossWerewolfAnim::AttackEnd);
				Montage_SetEndDelegate(MontageEndedDel, UppercutMontage);
			}
		}
	}
}

void UBossWerewolfAnim::PlayJumpAtk()
{
	if (AActor* Owner = TryGetPawnOwner()) {
		if (ABossWerewolf* Boss = Cast<ABossWerewolf>(Owner)) {
			if (JumpAtkMontage) {
				JumpAtkMontage->GetSectionStartAndEndTime(1, JumpAttackStartTime, JumpAttackEndTime);
				Montage_Play(JumpAtkMontage);
				FOnMontageEnded MontageEndedDel;
				MontageEndedDel.BindUObject(this, &UBossWerewolfAnim::AttackEnd);
				Montage_SetEndDelegate(MontageEndedDel, JumpAtkMontage);
			}
		}
	}
}

void UBossWerewolfAnim::PlayStoneUpAtk()
{
	if (AActor* Owner = TryGetPawnOwner()) {
		if (ABossWerewolf* Boss = Cast<ABossWerewolf>(Owner)) {
			if (StoneUpMontage) {
				Montage_Play(StoneUpMontage);
				FOnMontageEnded MontageEndedDel;
				MontageEndedDel.BindUObject(this, &UBossWerewolfAnim::AttackEnd);
				Montage_SetEndDelegate(MontageEndedDel, StoneUpMontage);
			}
		}
	}
}

void UBossWerewolfAnim::PlaySpreadWaveAtk()
{
	if (AActor* Owner = TryGetPawnOwner()) {
		if (ABossWerewolf* Boss = Cast<ABossWerewolf>(Owner)) {
			if (SpreadWaveMontage) {
				Montage_Play(SpreadWaveMontage);
				FOnMontageEnded MontageEndedDel;
				MontageEndedDel.BindUObject(this, &UBossWerewolfAnim::AttackEnd);
				Montage_SetEndDelegate(MontageEndedDel, SpreadWaveMontage);
			}
		}
	}
}

void UBossWerewolfAnim::AttackEnd(UAnimMontage* InMontage, bool bInterrupted)
{
	OnAttackFinished.ExecuteIfBound();
}

void UBossWerewolfAnim::SetAIAttackDelegate(const FAIBossAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}