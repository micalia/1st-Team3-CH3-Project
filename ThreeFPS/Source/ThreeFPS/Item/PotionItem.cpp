// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PotionItem.h"
#include "PotionItem.h"

void APotionItem::Use()
{
	Super::Use();

	UE_LOG(LogTemp, Warning, TEXT("포션 아이템 사용!!!"));
}
