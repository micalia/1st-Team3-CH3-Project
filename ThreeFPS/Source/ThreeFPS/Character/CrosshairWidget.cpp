// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CrosshairWidget.h"

void UCrosshairWidget::UpdateCrosshairSize(const float Speed)
{
	if (!TopCrosshair || !BottomCrosshair || !LeftCrosshair || !RightCrosshair)
		return;

	// 위치 계산 (간격은 Speed, FireRate, Recoil 값을 기반으로 설정)
	float Offset = FMath::Clamp(Speed * 0.1f, 0, 50.0f);

	// 각 라인의 위치 조정
	TopCrosshair->SetRenderTranslation(FVector2D(0, -Offset));
	BottomCrosshair->SetRenderTranslation(FVector2D(0, Offset));
	LeftCrosshair->SetRenderTranslation(FVector2D(-Offset, 0));
	RightCrosshair->SetRenderTranslation(FVector2D(Offset, 0));
}

void UCrosshairWidget::SetCrosshairVisibility(ESlateVisibility visibility)
{
	SetVisibility(visibility);
}
