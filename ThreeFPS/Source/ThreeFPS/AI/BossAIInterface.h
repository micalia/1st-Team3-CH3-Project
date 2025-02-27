// 보스 AI 담당 : 신설빈

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BossAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBossAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAIBossAttackFinished);

class THREEFPS_API IBossAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetAIAttackDelegate(const FAIBossAttackFinished& InOnAttackFinished) = 0;
};
