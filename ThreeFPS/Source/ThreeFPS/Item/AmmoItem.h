#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "AmmoItem.generated.h"

UCLASS()
class THREEFPS_API AAmmoItem : public AItemBase
{
	GENERATED_BODY()
	
public:
	void Use(AThreeFPSCharacter* player) override;
};
