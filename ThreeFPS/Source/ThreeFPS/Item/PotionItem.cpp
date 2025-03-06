#include "Item/PotionItem.h"
#include "Character/ThreeFPSCharacter.h"

void APotionItem::Use(AThreeFPSCharacter* player)
{
	Super::Use(player);

	player->IncreaseHealth(30);
}
