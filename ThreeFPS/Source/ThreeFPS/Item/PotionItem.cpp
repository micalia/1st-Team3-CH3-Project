#include "Item/PotionItem.h"
#include "Character/ThreeFPSCharacter.h"

void APotionItem::Use(AThreeFPSCharacter* player)
{
	Super::Use(player);

	UE_LOG(LogTemp, Warning, TEXT("포션 사용! 플레이어 체력 30 증가"));
	player->IncreaseHealth(30);
}
