#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "DrugItem.generated.h"

class AThreeFPSCharacter;

UCLASS()
class THREEFPS_API ADrugItem : public AItemBase
{
	GENERATED_BODY()
	
public:
	void Use(AThreeFPSCharacter* player) override;
};
