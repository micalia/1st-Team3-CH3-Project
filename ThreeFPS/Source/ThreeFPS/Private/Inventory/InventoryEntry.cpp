#include "Inventory/InventoryEntry.h"
#include "Item/ItemDatabase.h"
#include "Components/TextBlock.h"
#include "Inventory/TestCharacter.h"
#include "Components/Button.h"
#include "Inventory/InventoryWidget.h"
#include "Item/ItemBase.h"

void UInventoryEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UItemUIObject* UIItem = Cast<UItemUIObject>(ListItemObject);
	ItemName->SetText(FText::FromString(UIItem->ItemData.ItemName.ToString()));

	DropButton->OnClicked.Clear();
	DropButton->OnClicked.AddDynamic(this, &UInventoryEntry::DropItem);
}

void UInventoryEntry::DropItem()
{
	ATestCharacter* PlayerCharacter = Cast<ATestCharacter>(GetOwningPlayer()->GetPawn());
	FItemData ItemData = GetListItem<UItemUIObject>()->ItemData;
	GetWorld()->SpawnActor<AItemBase>(ItemData.Class, PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorForwardVector() * 100, FRotator());
	PlayerCharacter->Inventory.RemoveSingle(ItemData);
	PlayerCharacter->InventoryWidget->RefreshInventory(PlayerCharacter->Inventory);
	DropButton->OnClicked.Clear(); 
}
