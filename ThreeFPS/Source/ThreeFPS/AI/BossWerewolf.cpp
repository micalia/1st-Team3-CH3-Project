// Boss AI - Shin Seol Bin


#include "BossWerewolf.h"
#include "Components/CapsuleComponent.h"
#include "BossWerewolfAIController.h"
#include "Character/ThreeFPSCharacter.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"

ABossWerewolf::ABossWerewolf()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSK(TEXT("/Game/SB/Model/Werebear/Mesh/SK_Werebear_red.SK_Werebear_red"));
	if (tempSK.Succeeded()) {
		GetMesh()->SetSkeletalMesh(tempSK.Object);
	}
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -76));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FClassFinder<UBossWerewolfAnim> tempAnim(TEXT("/Game/SB/Blueprint/AB_BossWerewolf.AB_BossWerewolf_C"));
	if (tempAnim.Succeeded()) {
		GetMesh()->SetAnimClass(tempAnim.Class);
	}
	GetCapsuleComponent()->SetCapsuleHalfHeight(77);
	GetCapsuleComponent()->SetCapsuleRadius(71);

	static ConstructorHelpers::FClassFinder<ABossWerewolfAIController> tempAiCon(TEXT("/Game/SB/Blueprint/BP_BossWerewolfAIController.BP_BossWerewolfAIController_C"));
	if (tempAiCon.Succeeded()) {
		AIControllerClass = tempAiCon.Class;
	}

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ABossWerewolf::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void ABossWerewolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalculateDistance();

	if (jumpAttackOn == true) JumpAttackState();
	if (bTurnComplete == false) return;
	LookAtPlayerToAttack();
}

void ABossWerewolf::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/* 승현 추가 */
float ABossWerewolf::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrHp = FMath::Clamp(CurrHp - ActualDamage, 0.f, FullHp);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("액터 : %f"), CurrHp));

	if (CurrHp <= 0)
	{
		//Die()
	}
	return ActualDamage;
}

void ABossWerewolf::Init()
{
	CurrHp = FullHp;
}

void ABossWerewolf::CalculateDistance()
{
	if (auto Target = GetTarget()) {
		DistanceToTarget = (GetActorLocation() - Target->GetActorLocation()).Length();
	}
}

TObjectPtr<AThreeFPSCharacter> ABossWerewolf::GetTarget()
{
	if (TargetPtr->IsValidLowLevel()) {
		return TargetPtr;
	}
	else {
		for (TActorIterator<AThreeFPSCharacter> it(GetWorld()); it; ++it) {
			TargetPtr = *it;
		}
		if (TargetPtr->IsValidLowLevel()) {
			return TargetPtr;
		}
	}
	return nullptr;
}

FVector ABossWerewolf::CalculateBezier(float ratio, const FVector P0, const FVector P1, const FVector P2)
{
	FVector M0 = FMath::Lerp<FVector, float>(P0, P1, ratio);
	FVector M1 = FMath::Lerp<FVector, float>(P1, P2, ratio);
	FVector B = FMath::Lerp<FVector, float>(M0, M1, ratio);
	return B;
}

void ABossWerewolf::JumpAttackPath(const FVector InStartPos, const FVector InBetweenPos, const FVector InEndPos)
{
	LineLoc.Empty();
	float ratio = 1 / CurvePointCount;
	for (int32 i = 0; i <= (int32)CurvePointCount; i++)
	{
		FVector B = CalculateBezier(ratio * i, InStartPos, InBetweenPos, InEndPos);
		LineLoc.Add(B);
	}
}

void ABossWerewolf::MakeJumpAttackTrajectory()
{
	if (IsValid(TargetPtr)) {
		if (auto BossAnim = Cast<UBossWerewolfAnim>(GetMesh()->GetAnimInstance())) {
			JumpMovingTime = BossAnim->JumpAttackEndTime - BossAnim->JumpAttackStartTime;
			StartPos = GetActorLocation();
			FVector CapsuleCenter = TargetPtr->GetCapsuleComponent()->GetComponentLocation();
			float CapsuleHalfHeight = TargetPtr->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
			FVector BottomLocation = CapsuleCenter - FVector(0, 0, CapsuleHalfHeight);
			OriginEndPos = BottomLocation;

			FVector BetweenPos = FMath::Lerp(StartPos, OriginEndPos, 0.5);
			BetweenPos.Z += BetweenHeight;

			JumpAttackPath(StartPos, BetweenPos, OriginEndPos);
			jumpAttackOn = true;
		}
	}
}

void ABossWerewolf::JumpAttackState()
{
	PtoPCurrTime += GetWorld()->DeltaTimeSeconds;
	float PointToPointMoveTime = JumpMovingTime / CurvePointCount;

	if (PtoPCurrTime < PointToPointMoveTime) {
		float alpha = PtoPCurrTime / PointToPointMoveTime;
		SetActorLocation(FMath::Lerp(LineLoc[jumpAttackIdx], LineLoc[jumpAttackIdx + 1], alpha));
	}
	else {
		PtoPCurrTime = 0;
		jumpAttackIdx++;
		if (jumpAttackIdx + 1 == LineLoc.Num())
		{
			jumpAttackIdx = 0;
			jumpAttackOn = false;
			GetCharacterMovement()->GravityScale = 1;
		}
	}
}

void ABossWerewolf::SetNewGoalDirection()
{
	if (IsValid(TargetPtr)) {
		bTurnComplete = true;
		PlayerPosition = TargetPtr->GetActorLocation();
		PrevTurnAngle = GetActorRotation().Yaw;

		FVector Dir = PlayerPosition - GetActorLocation();
		ToPlayerDir = Dir.GetSafeNormal();
		float Dot = FVector::DotProduct(GetActorForwardVector(), ToPlayerDir);
		float Radian = FMath::Acos(Dot);
		float AngleToPlayer = Radian * 180 / 3.141592f;
		FVector Cross = FVector::CrossProduct(GetActorForwardVector(), ToPlayerDir);
		if (Cross.Z >= 0)
		{
			NextTurnAngle = AngleToPlayer;
		}
		else if (Cross.Z < 0)
		{
			NextTurnAngle = -AngleToPlayer;
		}
	}
}

void ABossWerewolf::LookAtPlayerToAttack()
{
	CurrentRotationTime += GetWorld()->GetDeltaSeconds();
	float alpha = CurrentRotationTime / RotationTime;
	if (alpha < 1)
	{
		float angle = PrevTurnAngle;
		angle += FMath::Lerp<float, float>(0, NextTurnAngle, alpha);
		SetActorRotation(FRotator(0.0f, angle, 0.0f).Quaternion());
	}
	else
	{
		SetActorRotation(FRotator(0.0f, PrevTurnAngle + NextTurnAngle, 0.0f).Quaternion());
		CurrentRotationTime = 0;
		bTurnComplete = false;
	}
}
