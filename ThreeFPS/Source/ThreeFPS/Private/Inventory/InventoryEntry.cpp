#include "Inventory/InventoryEntry.h"
#include "Item/ItemDatabase.h"
#include "Components/TextBlock.h"
#include "ThreeFPSCharacter.h"
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
	AThreeFPSCharacter* PlayerCharacter = Cast<AThreeFPSCharacter>(GetOwningPlayer()->GetPawn());
	FItemData ItemData = GetListItem<UItemUIObject>()->ItemData;
	GetWorld()->SpawnActor<AItemBase>(ItemData.Class, PlayerCharacter->GetActorLocation(), FRotator());
	// TODO : 플레이어 캐릭터에서 인벤토리 접근 가능하도록 연결 필요
	// PlayerCharacter->Inventory.Remove(ItemData);
	// PlayerCharacter->InventoryWidget->RefreshInventory(PlayerCharacter->Inventory);
	DropButton->OnClicked.Clear(); 
}
