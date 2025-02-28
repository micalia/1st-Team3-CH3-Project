// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZombieAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EZONBIE_BTT_ANI : uint8
{
    Btt_Att,
    Btt_Damage,
    Btt_Die
};
UCLASS()
class THREEFPS_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
    UZombieAnimInstance();
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    TArray<UAnimMontage*> AttMontageArr;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    UAnimMontage* DamageMontage;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    UAnimMontage* DieMontage;

    UFUNCTION(BlueprintCallable, Category = "Animation")
    float AnimationPlay(EZONBIE_ST Ani);

    //  애니메이션 인스턴스 초기화 (BeginPlay 대체)
    virtual void NativeInitializeAnimation() override;

    //  매 프레임마다 애니메이션 업데이트 (Tick 대체)
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    // 이동 속도 변수 (블루프린트에서 접근 가능하도록 설정)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    float Speed;

};
