#include "Intro/IntroHUD.h"
#include "Components/Button.h"
#include "Game/ThreeFPSPlayerController.h"
#include "Character/ThreeFPSCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Game/LevelManager.h"

void UIntroHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.Clear();
		StartButton->OnClicked.AddDynamic(this, &UIntroHUD::GameStart);
	}
	if (ExitButton)
	{
		ExitButton->OnClicked.Clear();
		ExitButton->OnClicked.AddDynamic(this, &UIntroHUD::GameExit);
	}
}

void UIntroHUD::GameStart()
{
	RemoveFromParent();

	ULevelManager* LevelManager = GetGameInstance()->GetSubsystem<ULevelManager>();

	if (IsValid(LevelManager))
	{
		FOnLevelLoadedDelegate Callback;
		Callback.BindDynamic(this, &UIntroHUD::OnLevelLoaded);
		LevelManager->LoadLevel(ELevelType::Tutorial, Callback);
	}
}

void UIntroHUD::GameExit()
{
	UKismetSystemLibrary::QuitGame(this, 0, EQuitPreference::Quit, false);
}

void UIntroHUD::OnLevelLoaded()
{
	ULevelManager* LevelManager = GetGameInstance()->GetSubsystem<ULevelManager>();
	if (IsValid(LevelManager))
	{
		LevelManager->UnLoadLevel(ELevelType::Intro, true);
	}

	if (AThreeFPSPlayerController* PlayerController = Cast<AThreeFPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->Possess(0);

		if (AThreeFPSCharacter* Player = Cast<AThreeFPSCharacter>(PlayerController->GetCharacter()))
		{
			Player->GameStart();
		}
	}
}

