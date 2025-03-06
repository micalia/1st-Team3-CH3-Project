#include "Item/AmmoItem.h"
#include "Character/ThreeFPSCharacter.h"

void AAmmoItem::Use(AThreeFPSCharacter* player)
{
	Super::Use(player);

	player->IncreaseAmmo(30);
}