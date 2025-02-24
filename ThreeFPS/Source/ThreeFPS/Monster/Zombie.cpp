// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "MonsterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterAIController.h"
AZombie::AZombie()
{
    PrimaryActorTick.bCanEverTick = false;
    GetCharacterMovement()->bUseRVOAvoidance = true; //true가 적용된 몬스터들은  AvoidanceConsiderationRadius 가중치에 따라 들러붙지않으려고 한다.
    GetCharacterMovement()->AvoidanceConsiderationRadius = 100.f;
    GetCapsuleComponent()->InitCapsuleSize(86.42f, 24.64f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    GetCapsuleComponent()->SetGenerateOverlapEvents(true);

    GetMesh()->MarkRenderStateDirty();
    GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -91.f), FRotator(0, 0, -90.f));
    GetMesh()->SetRelativeScale3D(FVector(1.f));
 
    //if (FMath::RandRange(0, 1) == 0)
    //{
    //    USkeletalMeshComponent* tMesh = CreateDefaultSubobject<USkeletalMeshComponent>("tMesh");
    //    ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/KSW/Resouces/zombie/mesh/Female/SK_Zombie_F02_01.SK_Zombie_F02_01'"));
    //    if (tempMesh.Succeeded())//"SkeletalMesh'/Game/KSW/Resouces/zombie/mesh/Female/SK_Zombie_F02_01.SK_Zombie_F02_01'"
    //    {
    //        GetMesh()->SetSkeletalMesh(tempMesh.Object);
    //        GetMesh()->MarkRenderStateDirty();
    //        GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -91.f), FRotator(0, 0, -90.f));
    //        GetMesh()->SetRelativeScale3D(FVector(1.f));
    //    }
    //}
    
    // /Script/Engine.AnimBlueprint'/Game/KSW/Blueprints/ABP_Zombie.ABP_Zombie'
    //static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("AnimBlueprint'/Game/KSW/Blueprints/ABP_Zombie.ABP_Zombie_C'"));

    //if (AnimBPClass.Succeeded())
    //{
    //    GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);  // 🚀 애님 블루프린트 적용
    //}
   

    ZombieState = EZONBIE_ST::IDLE;
    bPlayerDetected = false;

    AIControllerClass = AMonsterAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void AZombie::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Projectile"))
    {
        FVector AttackDirection = (GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal();
        ApplyRagdoll(AttackDirection);
        //UE_LOG(LogTemp, Warning, TEXT("몬스터가 %s와 오버랩 발생!"), *OtherActor->GetName());
    }
}

void AZombie::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AZombie::BeginPlay()
{
    Super::BeginPlay();
    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnCapsuleOverlap);


    /*
    int rand = FMath::RandRange(0, PantsArr.Num());
    if (PantsArr.Num() > rand)
        PantsArr[rand]->//SetVisibility(false);
        */
    //SetVisibility(false)
    //SetComponentTickEnabled(false) 
    //SetCollisionEnabled(ECollisionEnabled::NoCollision)

    //=========현재는 의미없는 로직임. ZombieState 가 쓰이고 있지 않음.======================
    ZombieState = EZONBIE_ST::DELAY;
    float RandActiveTime = FMath::RandRange(0.f, 1.5f);
    GetWorldTimerManager().SetTimer(
        GameStateHandle, [this, RandActiveTime]()
        {
            ZombieState = EZONBIE_ST::IDLE;
        },
        RandActiveTime,
        true
    );
 
}

void AZombie::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

}

void AZombie::Tick(float DeltaTime)
{
}

float AZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    return Damage;
}
void AZombie::ApplyRagdoll(FVector HitDirection)
{
    Super::ApplyRagdoll(HitDirection);

    //  캡슐 콜리전 비활성화 (레그돌이 제대로 작동하도록 설정)
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCapsuleComponent()->SetSimulatePhysics(true);

    //  이동 불가능하도록 설정 
    GetCharacterMovement()->StopMovementImmediately();
    GetCharacterMovement()->DisableMovement();

    //  스켈레탈 메시를 물리(Ragdoll) 상태로 변경
    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

    //  HitDirection 방향으로 힘을 가함 (살짝 뒤로 날아가도록 설정)
    FVector ImpulseForce = HitDirection * 4000.0f + FVector(0, 0.f, 2000.0f); // Y축으로 밀고 위로 튕기기// + FVector(1, 0.f, 0.0f)
    GetMesh()->AddImpulse(ImpulseForce, NAME_None, true);

    // FTimerHandle DestroyTimerHandle;
    // GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AMonster::Destroy, 3.0f, false);
}