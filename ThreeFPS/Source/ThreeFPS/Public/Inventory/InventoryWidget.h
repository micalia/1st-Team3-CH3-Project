#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemDatabase.h"
#include "InventoryWidget.generated.h"

class UListView;

UCLASS()
class THREEFPS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void RefreshInventory(const TArray<FItemData>& Inventory);

private:
	UPROPERTY(meta = (BindWidget))
	UListView* ItemList;
};
