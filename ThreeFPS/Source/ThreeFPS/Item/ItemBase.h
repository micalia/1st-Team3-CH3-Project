#pragma once
#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "ItemBase.generated.h"

class AThreeFPSCharacter;

UCLASS()
class THREEFPS_API AItemBase : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	virtual void Use(AThreeFPSCharacter* player);
};
