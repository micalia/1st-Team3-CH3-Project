#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionWidget.generated.h"

class UTextBlock;
UCLASS()
class THREEFPS_API UMissionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	FString newMissionToSet;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* missionText;

	UFUNCTION(BlueprintCallable)
	void UpdateMissionText(FString text);
};
