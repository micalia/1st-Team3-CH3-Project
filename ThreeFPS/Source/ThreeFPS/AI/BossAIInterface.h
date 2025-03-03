// Boss AI - 신설빈

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BossAIInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBossAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAIBossAttackFinished);

class THREEFPS_API IBossAIInterface
{
	GENERATED_BODY()

public:
	virtual void SetAIAttackDelegate(const FAIBossAttackFinished& InOnAttackFinished) = 0;
};
