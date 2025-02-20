#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IntroHUD.generated.h"

class UButton;

UCLASS()
class THREEFPS_API UIntroHUD : public UUserWidget
{
	GENERATED_BODY()

public:
// 위젯이 화면이 생성될 때 자옫으로 실행되는 초기화 함수
	virtual void NativeConstruct() override;

	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;
	UPROPERTY(meta = (BindWidget))
	UButton* SettingButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UFUNCTION()
	void GameStart();

	UFUNCTION()
	void Setting();

	UFUNCTION()
	void GameExit();
};
