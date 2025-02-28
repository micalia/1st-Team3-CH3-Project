#include "Loading/LoadingUIUserWidget.h"

void ULoadingUIUserWidget::PlayFadeInAnimation(bool IsReverse)
{
	if (FadeIn)
	{
		if (!IsReverse)
		{
			PlayAnimation(FadeIn);
		}
		else
		{
			PlayAnimationReverse(FadeIn);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FadeIn Failed"));
	}
}
