// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "MonsterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstance.h"
#include "DrawDebugHelpers.h"
#include "ZombieAnimInstance.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/DamageType.h"       // FPointDamageEvent, FRadialDamageEvent
#include "Engine/DamageEvents.h" //
#include "Components/PrimitiveComponent.h"  // UPrimitiveComponent (충돌체 및 물리 효과)
#include "Components/DecalComponent.h"
#include "Materials/MaterialInterface.h"

AZombie::AZombie()
{
    PrimaryActorTick.bCanEverTick = true;
    GetCharacterMovement()->bUseRVOAvoidance = true; //true가 적용된 몬스터들은  AvoidanceConsiderationRadius 가중치에 따라 들러붙지않으려고 한다.
    GetCharacterMovement()->AvoidanceConsiderationRadius = 100.f;
    GetCapsuleComponent()->InitCapsuleSize(86.42f, 24.64f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    GetCapsuleComponent()->SetGenerateOverlapEvents(true);

    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));//DetectionSphere = NewObject<USphereComponent>(this);
    if (DetectionSphere)
    {
        DetectionSphere->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
        DetectionSphere->SetSphereRadius(60.f);
        DetectionSphere->SetRelativeLocation(FVector(0.f, 30.f, 100.f));//40.f, 0.f, 20.f
        DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        DetectionSphere->SetCollisionObjectType(ECC_WorldDynamic);
        DetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
        DetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
        DetectionSphere->SetCollisionProfileName(TEXT("Trigger"));
    }
   
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshAsset(TEXT("SkeletalMesh'/Game/KSW/Resouces/zombie/mesh/male/AA_SK_Zombie_M02_01.AA_SK_Zombie_M02_01'"));
    if (BodyMeshAsset.Succeeded())
        GetMesh()->SetSkeletalMesh(BodyMeshAsset.Object);

   // GetMesh()->MarkRenderStateDirty();
    GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90.f), FRotator(0.f, -90.f, 0.f));
    GetMesh()->SetRelativeScale3D(FVector(1.f));
    
    PantsMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PantsMesh"));
    PantsMeshComp->SetupAttachment(GetMesh()); 
    PantsMeshComp->SetLeaderPoseComponent(GetMesh()); // 본을 공유

    
    ShirtMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShirtMesh"));
    ShirtMeshComp->SetupAttachment(GetMesh());
    ShirtMeshComp->SetLeaderPoseComponent(GetMesh());

    HairMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
    HairMeshComp->SetupAttachment(GetMesh());
    HairMeshComp->SetLeaderPoseComponent(GetMesh());

    HairStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HairStaticMesh"));
    HairStaticMeshComp->SetupAttachment(GetMesh(), FName("headSocket"));
    FQuat NewRotation = FQuat(FRotator(90.f, -90.f, -90.f));
    HairStaticMeshComp->SetRelativeRotation(NewRotation);
    HairStaticMeshComp->SetRelativeLocation(FVector(-164.7f, 0.f, 0.f));
    HairStaticMeshComp->SetRelativeScale3D(FVector(1.f, 1.f, -1.f));

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AMonsterAIController::StaticClass();

    FullHp =60;
    CurrHp = FullHp;
    ImpulseStrength = 1.f;
}
void AZombie::BeginPlay()
{
    Super::BeginPlay();

    /*GetCapsuleComponent()*/GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnCapsuleOverlap);
    DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnDetectionOverlap);
    DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AZombie::OnDetectionEndOverlap);
    DisableDetection();

    VariousJombie();//Random Custom 

   // GetCharacterMovement()->MaxWalkSpeed = FMath::RandRange(50.f,200.f);


   // DecalMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("MaterialInstanceConstant'/Game/KSW/Resouces/Decals/Splashers/Decal_BB_Inst.Decal_BB_Inst'"));

}

void TestSp()
{

}
void AZombie::SetClothingMeshs(USkeletalMesh* Pants, USkeletalMesh* Shirt, USkeletalMesh* Hair , UStaticMesh* HairStatic )
{
    if(Pants)
        PantsMeshComp->SetSkeletalMesh(Pants);
    if(Shirt)
        ShirtMeshComp->SetSkeletalMesh(Shirt);
    if(Hair)
        HairMeshComp->SetSkeletalMesh(Hair);
    if (HairStatic)
        HairStaticMeshComp->SetStaticMesh(HairStatic);
}

EPATROLTYPE AZombie::GetterPatrolType() const
{
    return PatrolType;
}

void AZombie::SetPatrolType(EPATROLTYPE Type)
{
    PatrolType = Type;
}

void AZombie::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnCapsuleOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    if (OtherActor && OtherActor->ActorHasTag("Projectile"))
    {
        FVector AttackDirection = (GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal();
        ApplyRagdoll(AttackDirection);
        //UE_LOG(LogTemp, Warning, TEXT("몬스터가 %s와 오버랩 발생!"), *OtherActor->GetName());
    }
}
void AZombie::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnCapsuleEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
}
void AZombie::OnDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnDetectionEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
    if (OtherActor == TargetActor)
    {
        bDetectionOverlapping = false;
        bAttackTimming = false;
        TargetActor = nullptr;
       // UE_LOG(LogTemp, Warning, TEXT("몬스터가 %s와 End오버랩 !"), *OtherActor->GetName());
    }
}
void AZombie::OnDetectionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnDetectionOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
   
    if (OtherActor && OtherActor->ActorHasTag("Player") )
    {
        bDetectionOverlapping = true;
        TargetActor = Cast<ACharacter>(OtherActor);
       // UE_LOG(LogTemp, Warning, TEXT("몬스터가 %s와 오버랩 발생!"), *OtherActor->GetName());
       // FVector CurPos = GetActorLocation() + DetectionSphere->GetRelativeLocation();
       // float Distance = FVector::DistXY(CurPos, TargetActor->GetActorLocation());
       // UE_LOG(LogTemp, Warning, TEXT("Distance:%f/ SphereRadius:"), Distance, DetectionSphere->GetScaledSphereRadius());
    }
}
void AZombie::EnableDetection()
{
    Super::EnableDetection(); // Show Debug Line Sphere
   // UE_LOG(LogTemp, Warning, TEXT("EnableDetection()!!!"));

    if (DetectionSphere && bDebugDetectionSphere)
    {
        DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
       // DrawDebugSphere(GetWorld(), DetectionSphere->GetComponentLocation(),
      //  DetectionSphere->GetScaledSphereRadius(), 12, FColor::Red, false, 3.f, 0, 2.f);
    }
}

void AZombie::DisableDetection()
{
    Super::DisableDetection();
    if (DetectionSphere)
        DetectionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    bDetectionOverlapping = false;
    bAttackTimming = false;
    TargetActor = nullptr;
}
float AZombie::Attack()
{
    Super::Attack();

    UZombieAnimInstance* Anim = Cast < UZombieAnimInstance>(GetMesh()->GetAnimInstance());
    float AniPlayLength = -1.f;
    if (Anim )
        AniPlayLength = Anim->AnimationPlay(EANIM_MONTAGE::ATTACK);
    return AniPlayLength;
}


void AZombie::AttackTimming(int AttType)
{
    Super::AttackTimming(AttType);

    Power = AttackPowerArr[AttType];
    bAttackTimming = true;
}

void AZombie::Tick(float DeltaTime)
{
    if ( bDetectionOverlapping )
    {
        if (bAttackTimming)
        {
            if (TargetActor&& TargetActor->ActorHasTag("Player"))
            {
                //FVector CurPos = GetActorLocation() +  DetectionSphere->GetRelativeLocation();
                //float Distance = FVector::DistXY(CurPos, TargetActor->GetActorLocation());
                //UE_LOG(LogTemp, Warning, TEXT("Distance:%f/ SphereRadius:"), Distance, DetectionSphere->GetScaledSphereRadius());
                //if (150.f >= Distance)

                float  ReceiveApplyDamage = UGameplayStatics::ApplyDamage(TargetActor, Power,GetController(), this, UDamageType::StaticClass());
                  
               // UE_LOG(LogTemp, Warning, TEXT("ATT:%f"),Power);

                
                /* FPointDamageEvent PointDamageEvent;
                PointDamageEvent.HitInfo = HitResult;

                AActor* DamageCauser = this;
                UGameplayStatics::ApplyPointDamage(TargetActor, Power, ShotDirection,PointDamageEvent.HitInfo, GetController(), DamageCauser,
                    UDamageType::StaticClass());*/
            }
            bAttackTimming = false;
        }
    }
}
float AZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("액터 : %f"), Hp));

    if (CurrHp <= 0)
        return 0.f;

    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    CurrHp = FMath::Clamp(CurrHp - ActualDamage, 0.f, FullHp);


    UZombieAnimInstance* animInst = Cast< UZombieAnimInstance>(GetMesh()->GetAnimInstance());
    if (!animInst)
        return 0.f;
    
    // 히트 결과를 받아올 FHitResult 생성
    FHitResult HitResult;
    if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
    {
        const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
        HitResult = PointDamageEvent->HitInfo;
    }
    else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
    {
        const FRadialDamageEvent* RadialDamageEvent = static_cast<const FRadialDamageEvent*>(&DamageEvent);
        if (RadialDamageEvent->ComponentHits.Num() > 0)
            HitResult = RadialDamageEvent->ComponentHits[0];
    }
    if (HitResult.GetComponent())//HitResult.IsValidBlockingHit())
    {
        float DamageAniTime = animInst->AnimationPlay(EANIM_MONTAGE::DAMAGE);
        PauseMoveForDamage(DamageAniTime, HitResult);
    }
    return ActualDamage;
}
void AZombie::Die()
{
    Super::Die();
   
    UZombieAnimInstance* animInst = Cast< UZombieAnimInstance>(GetMesh()->GetAnimInstance());
    if (!IsValid(animInst))
        return;

    GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandler);
    AMonsterAIController* MAIController = Cast<AMonsterAIController>(GetController());//StopMovement();
    if (MAIController)
    {
        MAIController->UnPossess();
        MAIController->StopMovement();
        MAIController->Destroy();
        UBehaviorTreeComponent* BT = Cast<UBehaviorTreeComponent>(MAIController->GetBrainComponent());//(TEXT("TakeDamage"));
        if (BT)
        {
            BT->Deactivate();//StopLogic(TEXT("ZombieDie"));
            UE_LOG(LogTemp, Log, TEXT(" MAIController->Destroy()"));
        }
    }
    
    GetCharacterMovement()->StopMovementImmediately();
    GetCharacterMovement()->DisableMovement();
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
    DisableDetection();
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
    //GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("pelvis"), true, true);
 

    float aniLength = animInst->AnimationPlay(EANIM_MONTAGE::DIE) - 0.3f;
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle, ([this, animInst, aniLength]() {
            if (IsValid(animInst))
                 animInst->Montage_Pause(animInst->DieMontage);
           // UE_LOG(LogTemp, Warning, TEXT("Montage_Pause! : %f"), aniLength);
            // animInst->Montage_SetPlayRate(animInst->DieMontage, 0.0f);
            }), aniLength, false);

    FTimerHandle TimerHandle2;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle2, ([this]() {
            UE_LOG(LogTemp, Warning, TEXT("Destroy !!!"));
            if(!IsPendingKillPending())
                 Destroy();
            }), 10.f, false);
  
}
void AZombie::PauseMoveForDamage(float PauseTime,FHitResult HitResult)
{
    AMonsterAIController* MAIController = Cast<AMonsterAIController>(GetController());//StopMovement();
    UBehaviorTreeComponent* BT = nullptr;
    if (MAIController)
    {
        BT = Cast<UBehaviorTreeComponent>(MAIController->GetBrainComponent());//(TEXT("TakeDamage"));
        if (IsValid(BT))
        {
            BT->PauseLogic(TEXT("TakeDamage"));
            if (IsValid(MAIController))
                 MAIController->StopMovement();
        }
    }

    UPrimitiveComponent* HitComponent = HitResult.GetComponent();
    FName HitBoneName = HitResult.BoneName;
    if ("pelvis" == HitBoneName  || "None" == HitBoneName )//
        HitBoneName = FName("spine_02");
    UE_LOG(LogTemp, Log, TEXT("HitBoneName : %s"), *HitBoneName.ToString());
    //FVector ImpulseDirection = HitResult.ImpactNormal * -1.0f;
    //FVector Impulse = ImpulseDirection * 5000.f;//ImpulseStrength;
    //UE_LOG(LogTemp, Log, TEXT("Impulse: %f / %f / %f"), Impulse.X, Impulse.Y, Impulse.Z);

 
    GetMesh()->SetAllBodiesBelowSimulatePhysics(HitBoneName, true, true);
    GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(HitBoneName, 0.5f, true, true);
    GetMesh()->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector, false, HitBoneName);
    //GetMesh()->AddImpulseAtLocation(Impulse, HitResult.ImpactPoint, HitBoneName);
    UE_LOG(LogTemp, Log, TEXT("Applied impulse to bone: %s"), *HitBoneName.ToString());
    if (HitImpactEffect)
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitImpactEffect, HitResult.ImpactPoint, FRotator::ZeroRotator,true);

    GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandler);
    if (CurrHp <= 0)
    {
        Die();
        float Rand = FMath::RandRange(0.f, 180.f);
        float XPos = FMath::RandRange(-60.f, 60.f);
        float YPos = FMath::RandRange(-60.f, 60.f);
        FVector BloodPos = FVector(GetActorLocation().X + XPos, GetActorLocation().Y + YPos, GetActorLocation().Z);
        SpawnDecalAtLocation( BloodPos + GetMesh()->GetRelativeLocation(), FRotator(90.0f, Rand, 0.f ), 30.f );
    }
    else {
        
        GetWorld()->GetTimerManager().SetTimer(
            DamageTimerHandler, ([this, MAIController, BT]() {

                //GetMesh()->SetBodySimulatePhysics(BoneName, false);
                //GetMesh()->ResetAllBodiesSimulatePhysics();
                if (CurrHp > 0)
                {
                    if (IsValid(BT))
                    {
                        BT->ResumeLogic(TEXT("TakeDamage"));
                        if(IsValid(MAIController))
                                MAIController->ChaseAfterDamage();
                    }
                }
                }), PauseTime + 0.f, false
        );
    }

}
void AZombie::SpawnDecalAtLocation(FVector Location, FRotator Rotation, float LifeSpan)
{
    if (DecalMaterial)
    {
        // 데칼 사이즈 설정 (XYZ 크기)
        FVector DecalSize = FVector( FMath::RandRange(90.f, 130.f), FMath::RandRange(90.f, 130.f), 100.0f);

        // 데칼 컴포넌트 생성
        UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
            GetWorld(),
            DecalMaterial,   // 데칼에 사용할 머티리얼
            DecalSize,              // 데칼의 크기
            Location,               // 데칼을 표시할 위치
            Rotation,               // 데칼의 회전값
            LifeSpan                // 데칼의 생존 시간 (0이면 영구 표시)
        );

        if (Decal)
        {
            // 추가적인 데칼 설정
            Decal->SetFadeScreenSize(0.0f); // 카메라와의 거리에서 페이드 아웃되는 크기 설정
            Decal->SetFadeOut(LifeSpan - 1.0f, 2.0f); // 사라질 때 페이드 효과
            //UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(DecalMaterial, this);
            //if (DynamicMaterial)
            //    DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::Red); // 'Color' 파라미터를 빨간색으로 변경
            //    Decal->SetDecalMaterial(DynamicMaterial);
        }
    }
    else
        UE_LOG(LogTemp, Error, TEXT("Error Decal Material "));
}
#pragma region //VariousJombie
void AZombie::VariousJombie()
{
    TMap<FString, FString> CostumeMap =  // Shirt 4개, Pants 2개 , hair 5개,
    {
        {TEXT("Shirt0"), TEXT("SkeletalMesh'/Game/KSW/Resouces/zombie/mesh/male/Clothing/Singlet/SK_Clth_Singlet_M02.SK_Clth_Singlet_M02'")},
        {TEXT("Shirt1"), TEXT("SkeletalMesh'/Game/KSW/Resouces/zombie/mesh/male/Clothing/Tshirt/SK_Clth_Tshirt_M02.SK_Clth_Tshirt_M02'")},
        {TEXT("Shirt2"), TEXT("SkeletalMesh'/Game/KSW/Resouces/zombie/mesh/Female/Clothing/OutfitCheerleader/SK_Clth_Singlet_Cheerleader_F02.SK_Clth_Singlet_Cheerleader_F02'")},
        {TEXT("Shirt3"), TEXT("SkeletalMesh'/Game/KSW/Resouces/zombie/mesh/Female/Clothing/OutfitLingerie/SK_Clth_Bra_Lingerie_F02.SK_Clth_Bra_Lingerie_F02'")},

        {TEXT("Pants0"),TEXT("SkeletalMesh'/Game/KSW/Resouces/zombie/mesh/male/Clothing/LongJeans/SK_Clth_LongPants_M02.SK_Clth_LongPants_M02'")},
        {TEXT("Pants1"),TEXT("SkeletalMesh'/Game/KSW/Resouces/zombie/mesh/Female/Clothing/OutfitCheerleader/SK_Clth_Skirt_Cheerleader_F02.SK_Clth_Skirt_Cheerleader_F02'")},
        {TEXT("Hair0"), TEXT("StaticMesh'/Game/KSW/Resouces/zombie/mesh/male/Hair/SM_Hair_M02.SM_Hair_M02'")},
        {TEXT("Hair1"), TEXT("StaticMesh'/Game/KSW/Resouces/zombie/mesh/male/Hair/SM_HairCap_M02.SM_HairCap_M02'")},
        {TEXT("Hair2"), TEXT("StaticMesh'/Game/KSW/Resouces/zombie/mesh/male/Hair/SM_HairCombined_M02.SM_HairCombined_M02'")},
        {TEXT("Hair3"), TEXT("SkeletalMesh'/Game/KSW/Resouces/zombie/mesh/Female/Hair/SK_Hair_F02.SK_Hair_F02'")},
        {TEXT("Hair4"), TEXT("SkeletalMesh'/Game/KSW/Resouces/zombie/mesh/Female/Hair/SK_HairCap_F02.SK_HairCap_F02'")},
    };

    bool bSelectFeMale = FMath::RandRange(0, 1) == 0 ? true : false;
    FString BodyMeshFilePath = "";
    if (bSelectFeMale)
        BodyMeshFilePath = TEXT("SkeletalMesh'/Game/KSW/Resouces/zombie/mesh/Female/SK_Zombie_F02_01.SK_Zombie_F02_01'");
    else
        BodyMeshFilePath = TEXT("SkeletalMesh'/Game/KSW/Resouces/zombie/mesh/male/AA_SK_Zombie_M02_01.AA_SK_Zombie_M02_01'");

    USkeletalMesh* BodyMesh = LoadObject<USkeletalMesh>(nullptr, *BodyMeshFilePath);
    if (BodyMesh)
        GetMesh()->SetSkeletalMesh(BodyMesh);
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Error! Create Body "));
        return;
    }

    bool bNude = FMath::RandRange(0, 1) == 0 ? true : false;  //누드 바디면 기본 바디로만 진행(속웃상태) 
    if (bNude)
    {
        USkeletalMesh* BraMesh = nullptr;
        USkeletalMesh* FhairMesh = nullptr;
        UStaticMesh* MhairMesh = nullptr;
        FString HairKey = "";
        FString Mhair = "";
        int randHairType = FMath::RandRange(0, 2);
        if (bSelectFeMale)//여성이면 브라자 입힘.
        {
            BraMesh = LoadObject<USkeletalMesh>(nullptr, *CostumeMap["Shirt3"]);
            if (randHairType != 0)
            {
                HairKey = "Hair" + FString::FromInt(FMath::RandRange(3, 4));
                FhairMesh = LoadObject<USkeletalMesh>(nullptr, *CostumeMap[HairKey]);
            }
        }
        else
        {
            if (randHairType != 0)
            {
                HairKey = "Hair" + FString::FromInt(FMath::RandRange(0, 2));
                MhairMesh = LoadObject<UStaticMesh>(nullptr, *CostumeMap[HairKey]);
            }
        }
        SetClothingMeshs(nullptr, BraMesh, FhairMesh, MhairMesh);
        return;
    }

    int32 HairIdx = -1;
    int32 ShirtIdx = -1;
    int32 PantsIdx = -1;
    if (bSelectFeMale)
    {
        HairIdx = FMath::RandRange(3, 4);
        ShirtIdx = FMath::RandRange(2, 3);
        PantsIdx = 1;
    }
    else
    {
        HairIdx = FMath::RandRange(0, 2);
        ShirtIdx = FMath::RandRange(0, 1);
        PantsIdx = 0;
    }

    FString FileLoadPathHair = "Hair" + FString::FromInt(HairIdx);
    FString FileLoadPathShirt = "Shirt" + FString::FromInt(ShirtIdx);
    FString FileLoadPathPants = "Pants" + FString::FromInt(PantsIdx);

    UStaticMesh* HairStaticMesh = nullptr;
    USkeletalMesh* HairMesh = nullptr;
    USkeletalMesh* ShirtMesh = LoadObject<USkeletalMesh>(nullptr, *CostumeMap[FileLoadPathShirt]);
    USkeletalMesh* PantsMesh = LoadObject<USkeletalMesh>(nullptr, *CostumeMap[FileLoadPathPants]);

    if (!bSelectFeMale)
    {
        if (FMath::RandRange(0, 1) == 1)
        {
            //일단 남자 청바지만 맵이 두종류임
            UMaterialInstance* NewMaterial = LoadObject<UMaterialInstance>(nullptr, TEXT("MaterialInstanceConstant'/Game/KSW/Resouces/zombie/Materials/ZombieM02/Clothing/LongJeans/02BlueDirty/MI_Clth_LongJeans_M02_02.MI_Clth_LongJeans_M02_02'"));
            if (NewMaterial)
                PantsMeshComp->SetMaterial(0, NewMaterial);
        }
    }

    if (bSelectFeMale)
        HairMesh = LoadObject<USkeletalMesh>(nullptr, *CostumeMap[FileLoadPathHair]);
    else
        HairStaticMesh = LoadObject<UStaticMesh>(nullptr, *CostumeMap[FileLoadPathHair]);

    SetClothingMeshs(PantsMesh, ShirtMesh, HairMesh, HairStaticMesh);

    //FString LogString = bSelectFeMale ? "True" : "False";
    //UE_LOG(LogTemp, Warning, TEXT("bSelectFeMale : %s"), *LogString);
}
#pragma endregion


void AZombie::ApplyRagdoll(FVector HitDirection)
{
    Super::ApplyRagdoll(HitDirection);

    AMonsterAIController* MAIController = Cast<AMonsterAIController>(GetController());//StopMovement();
    UBehaviorTreeComponent* BT = nullptr;
    if (MAIController)
    {
        BT = Cast<UBehaviorTreeComponent>(MAIController->GetBrainComponent());
      //BT->StopLogic(TEXT("Die"));
        BT->Deactivate();
        MAIController->StopMovement();
    }

    //  캡슐 콜리전 비활성화 (레그돌이 제대로 작동하도록 설정)
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //GetCapsuleComponent()->SetSimulatePhysics(true);

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

void AZombie::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}
