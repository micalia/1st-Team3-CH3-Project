#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDatabase.generated.h"

class AItemBase;

USTRUCT()
struct FItemData
{
	GENERATED_BODY()

	bool operator == (const FItemData& Other) const
	{
		return ItemName == Other.ItemName;
	}

	UPROPERTY(EditDefaultsOnly)
	FName ItemName;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AItemBase> Class;
};

UCLASS()
class THREEFPS_API UItemUIObject : public UObject
{
	GENERATED_BODY()

public:
	FItemData ItemData;
};

UCLASS()
class THREEFPS_API UItemDatabase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FItemData> Items;
};
