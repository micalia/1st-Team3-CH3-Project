#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingUIUserWidget.generated.h"

UCLASS()
class THREEFPS_API ULoadingUIUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PlayFadeInAnimation(bool IsReverse);

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeIn;
};
