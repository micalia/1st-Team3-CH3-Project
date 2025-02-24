// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZonbieAnimInstance.generated.h"


UCLASS()
class THREEFPS_API UZonbieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UZonbieAnimInstance();

    //  애니메이션 인스턴스 초기화 (BeginPlay 대체)
    virtual void NativeInitializeAnimation() override;

    //  매 프레임마다 애니메이션 업데이트 (Tick 대체)
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
   
    

protected:
    // 이동 속도 변수 (블루프린트에서 접근 가능하도록 설정)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    float Speed;

};
