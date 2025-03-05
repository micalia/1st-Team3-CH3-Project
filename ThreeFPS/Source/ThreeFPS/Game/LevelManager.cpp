#include "Game/LevelManager.h"
#include "Kismet/GameplayStatics.h"
#include "ThreeFPSGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Loading/LoadingUIUserWidget.h"


FString ULevelManager::GetLevelName(ELevelType LevelType)
{
	switch (LevelType)
	{
		case ELevelType::Intro: return TEXT("01_Intro_Level");
		case ELevelType::Tutorial: return TEXT("02_Tutorial_Level");
		case ELevelType::FirstChapter: return TEXT("03_First_Chapter_Level");
		case ELevelType::SecondChapter: return TEXT("04_Second_Chapter_Level");
		case ELevelType::BossCinema: return TEXT("05_BossCinema_Level");
		case ELevelType::BossBattle: return TEXT("06_BossBattle_Level");
		default: return TEXT("");
	}
}

void ULevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
	LoadingWidget = nullptr;
}

void ULevelManager::LoadLevel(ELevelType LevelType, FOnLevelLoadedDelegate& OnLoadedCallback)
{
	OnLevelLoaded = OnLoadedCallback;

	FString LevelName = GetLevelName(LevelType);
	if (LevelName.IsEmpty()) return;
	
	if (LevelType != ELevelType::Intro)
	{
		// 1. 로딩 UI 활성화
		if (IsValid(LoadingWidget))
		{
			LoadingWidget->AddToViewport(2);
			// UI 페이드인 애니메이션 실행
			Cast<ULoadingUIUserWidget>(LoadingWidget)->PlayFadeInAnimation(false);

			UKismetSystemLibrary::Delay(this, 1.0f, FLatentActionInfo());
		}
	}

	// 2. 비동기 로딩
	UWorld* World = GetGameInstance()->GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("World is Error!"));
		return;
	}
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "OnLevelLoadCompleted"; // 콜백 메서드 이름
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = FMath::Rand();
	CurrentLevel = LevelType;
	UGameplayStatics::LoadStreamLevel(World, FName(*LevelName), true, true, LatentInfo);
}

void ULevelManager::UnLoadLevel(ELevelType LevelType, bool isUnLoad)
{
	FString LevelName = GetLevelName(LevelType);
	if (LevelName.IsEmpty()) return;

	ULevelStreaming* LoadedLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), FName(*LevelName));
	if (LoadedLevel)
	{
		// 다시 방문하지 않으면 완전 해제
		if (isUnLoad)
		{
			LoadedLevel->SetShouldBeLoaded(false);
		}
		// 다시 방문 가능하면 비활성화
		LoadedLevel->SetShouldBeVisible(false);
	}
}

void ULevelManager::OnLevelLoadCompleted()
{
	if (IsValid(LoadingWidget))
	{
		UWorld* World = GetGameInstance()->GetWorld();
		if (!IsValid(World))
		{
			UE_LOG(LogTemp, Error, TEXT("World is Error!"));
			return;
		}
		FTimerHandle handle;
		World->GetTimerManager().SetTimer(
			handle,
			this,
			&ULevelManager::PlayFadeOut,
			1.0f,
			false
		);
	}
}

void ULevelManager::PlayFadeOut()
{
	if (IsValid(LoadingWidget))
	{
		Cast<ULoadingUIUserWidget>(LoadingWidget)->PlayFadeInAnimation(true);
		UKismetSystemLibrary::Delay(this, 1.0f, FLatentActionInfo());
	}

	if (OnLevelLoaded.IsBound())
	{
		OnLevelLoaded.Execute();
	}
}

ELevelType ULevelManager::GetCurrentLevelType()
{
	return CurrentLevel;
}
