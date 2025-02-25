#include "Intro/IntroHUD.h"
#include "Components/Button.h"
#include "Game/ThreeFPSPlayerController.h"
#include "Character/ThreeFPSCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

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

void UIntroHUD::GameExit()
{
	UKismetSystemLibrary::QuitGame(this, 0, EQuitPreference::Quit, false);
}
