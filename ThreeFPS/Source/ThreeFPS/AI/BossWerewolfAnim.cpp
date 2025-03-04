// Boss AI - 신설빈

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
	static ConstructorHelpers::FObjectFinder<UAnimMontage> tempDieMontage(TEXT("/Script/Engine.AnimMontage'/Game/SB/Animation/BossWearAnim/Use/AM_Die.AM_Die'"));
	if (tempDieMontage.Succeeded()) {
		DieMontage = tempDieMontage.Object;
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
				SetAnimState(EBossWerewolfState::Uppercut);
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
				SetAnimState(EBossWerewolfState::JumpAtk);
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
				SetAnimState(EBossWerewolfState::StoneUp);
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
				SetAnimState(EBossWerewolfState::SpreadWave);
				Montage_Play(SpreadWaveMontage);
				FOnMontageEnded MontageEndedDel;
				MontageEndedDel.BindUObject(this, &UBossWerewolfAnim::AttackEnd);
				Montage_SetEndDelegate(MontageEndedDel, SpreadWaveMontage);
			}
		}
	}
}

void UBossWerewolfAnim::PlaySwitchPhase()
{
	SetAnimState(EBossWerewolfState::SwitchPhase);
}

void UBossWerewolfAnim::PlayDie()
{
	if (AActor* Owner = TryGetPawnOwner()) {
		if (ABossWerewolf* Boss = Cast<ABossWerewolf>(Owner)) {
			if (DieMontage) {
				Montage_Play(DieMontage);
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