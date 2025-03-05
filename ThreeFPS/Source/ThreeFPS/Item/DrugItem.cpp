#include "Item/DrugItem.h"
#include "Character/ThreeFPSCharacter.h"

void ADrugItem::Use(AThreeFPSCharacter* player)
{
	Super::Use(player);

	UE_LOG(LogTemp, Warning, TEXT("포션 사용! 플레이어 체력 30 증가"));
	player->DecreaseMutation(30);
}