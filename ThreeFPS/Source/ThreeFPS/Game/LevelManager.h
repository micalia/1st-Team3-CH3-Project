#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Delegates/Delegate.h"
#include "LevelManager.generated.h"

UENUM(BlueprintType)
enum class ELevelType : uint8
{
	Intro UMETA(DisplayName = "Intro Level"),
	Tutorial UMETA(DisplayName = "Tutorial Level"),
	FirstChapter UMETA(DisplayName = "FirstChapter Level"),
	SecondChapter UMETA(DisplayName = "SecondChapter Level"),
	Boss UMETA(DisplayName = "Boss Level")
};

DECLARE_DYNAMIC_DELEGATE(FOnLevelLoadedDelegate);

UCLASS()
class THREEFPS_API ULevelManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	FOnLevelLoadedDelegate OnLevelLoaded;

	void LoadLevel(ELevelType LevelType, FOnLevelLoadedDelegate& OnLoadedCallback);

	UFUNCTION()
	void UnLoadLevel(ELevelType LevelType, bool IsUnLoad);

	UFUNCTION()
	void PlayFadeOut();

	UPROPERTY()
	UUserWidget* LoadingWidget;
private:
	UFUNCTION()
	FString GetLevelName(ELevelType LevelType);
	UFUNCTION()
	void OnLevelLoadCompleted();
	

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* FadeIn;
};
