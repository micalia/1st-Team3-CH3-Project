#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "PotionItem.generated.h"

class AThreeFPSCharacter;

UCLASS()
class THREEFPS_API APotionItem : public AItemBase
{
	GENERATED_BODY()
	
public:
	void Use(AThreeFPSCharacter* player) override;
};
