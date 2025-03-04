#include "Mission/MissionWidget.h"
#include "Components/TextBlock.h"


void UMissionWidget::UpdateMissionText(FString text)
{
	if (!text.IsEmpty())
	{
		missionText->SetText(FText::FromString(text));
	}
}
