#include "ThreeFPSGameMode.h"
#include "ThreeFPSPlayerController.h"
#include "ThreeFPS/Character/ThreeFPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Intro/IntroHUD.h"
#include "Loading/LoadingUIUserWidget.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "LevelManager.h"

AThreeFPSGameMode::AThreeFPSGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerFinder(TEXT("/Game/SHyeon/Blueprints/BP_ThreeFPSPlayerController"));
	if (PlayerControllerFinder.Succeeded())
	{
		PlayerControllerClass = PlayerControllerFinder.Class;
	}
}

void AThreeFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

	ULevelManager* LevelManager = GetGameInstance()->GetSubsystem<ULevelManager>();
	if (!IsValid(LevelManager))
	{
		UE_LOG(LogTemp, Error, TEXT("LevelManager nullptr"));
	}
	if (IsValid(LevelManager->LoadingWidget))
	{
		LevelManager->LoadingWidget->RemoveFromParent();
		LevelManager->LoadingWidget = nullptr;
	}

	if (IsValid(LoadingWidgetClass))
	{
		LevelManager->LoadingWidget = Cast<ULoadingUIUserWidget>(CreateWidget(GetWorld(), LoadingWidgetClass));
		FOnLevelLoadedDelegate OnIntroLoadedDelegate;
		OnIntroLoadedDelegate.BindDynamic(this, &AThreeFPSGameMode::SwitchToCineCamera);
		LevelManager->LoadLevel(ELevelType::Intro, OnIntroLoadedDelegate);
	}
}

void AThreeFPSGameMode::SwitchToCineCamera()
{
	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundCameras);

	for (AActor* Actor : FoundCameras)
	{
		if (Actor->GetName().Contains("CineCamera"))
		{
			ACameraActor* CineCamera = Cast<ACameraActor>(Actor);
			if (CineCamera)
			{
				if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
				{
					PlayerController->SetViewTargetWithBlend(CineCamera, 0.0f);
				}
			}
		}
	}
	SetIntroUI();
}

void AThreeFPSGameMode::SetIntroUI()
{
	if (IsValid(IntroHUDWidgetClass))
	{
		IntroHUDWidget = Cast<UIntroHUD>(CreateWidget(GetWorld(), IntroHUDWidgetClass));
		if (IntroHUDWidget)
		{
			IntroHUDWidget->AddToViewport(0);
			IntroHUDWidget->SetVisibility(ESlateVisibility::Visible);

			if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				PlayerController->bShowMouseCursor = true;
				PlayerController->SetInputMode(FInputModeUIOnly());
			}
		}
	}
}

void AThreeFPSGameMode::LoadingSecondChapter()
{
	ULevelManager* LevelManager = GetGameInstance()->GetSubsystem<ULevelManager>();
	if (!IsValid(LevelManager))
	{
		UE_LOG(LogTemp, Error, TEXT("LevelManager nullptr"));
	}
	if (IsValid(LevelManager->LoadingWidget))
	{
		LevelManager->LoadingWidget->RemoveFromParent();
		LevelManager->LoadingWidget = nullptr;
	}

	if (IsValid(LoadingWidgetClass))
	{
		LevelManager->LoadingWidget = Cast<ULoadingUIUserWidget>(CreateWidget(GetWorld(), LoadingWidgetClass));
		FOnLevelLoadedDelegate OnSecondLevelLoadedDelegate;
		OnSecondLevelLoadedDelegate.BindDynamic(this, &AThreeFPSGameMode::SecondLevelLoaded);
		LevelManager->LoadLevel(ELevelType::SecondChapter, OnSecondLevelLoadedDelegate);
	}
}

void AThreeFPSGameMode::SecondLevelLoaded()
{
	ULevelManager* LevelManager = GetGameInstance()->GetSubsystem<ULevelManager>();
	if (!IsValid(LevelManager))
	{
		UE_LOG(LogTemp, Error, TEXT("LevelManager nullptr"));
	}
	LevelManager->UnLoadLevel(ELevelType::Tutorial, true);
}

void AThreeFPSGameMode::LoadingBossChapter()
{
	ULevelManager* LevelManager = GetGameInstance()->GetSubsystem<ULevelManager>();
	if (!IsValid(LevelManager))
	{
		UE_LOG(LogTemp, Error, TEXT("LevelManager nullptr"));
	}
	if (IsValid(LevelManager->LoadingWidget))
	{
		LevelManager->LoadingWidget->RemoveFromParent();
		LevelManager->LoadingWidget = nullptr;
	}

	if (IsValid(LoadingWidgetClass))
	{
		LevelManager->LoadingWidget = Cast<ULoadingUIUserWidget>(CreateWidget(GetWorld(), LoadingWidgetClass));
		FOnLevelLoadedDelegate OnBossLevelLoadedDelegate;
		OnBossLevelLoadedDelegate.BindDynamic(this, &AThreeFPSGameMode::BossLevelLoaded);
		LevelManager->LoadLevel(ELevelType::BossBattle, OnBossLevelLoadedDelegate);
	}
}

void AThreeFPSGameMode::BossLevelLoaded()
{
	ULevelManager* LevelManager = GetGameInstance()->GetSubsystem<ULevelManager>();
	if (!IsValid(LevelManager))
	{
		UE_LOG(LogTemp, Error, TEXT("LevelManager nullptr"));
	}
}