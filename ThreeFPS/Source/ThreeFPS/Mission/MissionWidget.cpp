#include "Components/TextBlock.h"
#include "Mission/MissionWidget.h"

void UMissionWidget::UpdateMissionText(FString text)
{
	if (!text.IsEmpty())
	{
		missionText->SetText(FText::FromString(text));
	}
}
