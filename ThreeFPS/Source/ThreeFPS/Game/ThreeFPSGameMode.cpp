#include "ThreeFPSGameMode.h"
#include "ThreeFPSPlayerController.h"
#include "ThreeFPS/Character/ThreeFPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Intro/IntroHUD.h"
#include "Kismet/GameplayStatics.h"

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

	UE_LOG(LogTemp, Warning, TEXT("Start"));

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
