#include "Item/DrugItem.h"
#include "Character/ThreeFPSCharacter.h"

void ADrugItem::Use(AThreeFPSCharacter* player)
{
	Super::Use(player);

	player->DecreaseMutation(30);
}