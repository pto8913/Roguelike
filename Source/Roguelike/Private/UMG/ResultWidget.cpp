// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/UMG/ResultWidget.h"
#include "Public/RoguelikeGameInstance.h"

#include "TextBlock.h"
#include "Button.h"
#include "Engine.h"

void UResultWidget::OpenResult()
{
	
	if (!IsInViewport())
	{
		if (GetGameInstance() != nullptr)
		{
			AddToViewport();
			AddDelegate();
			GameInstance->GamePause(FInputModeUIOnly());

			SetResult();

			PlayAnimation(FadeIn);
		}
	}
}

void UResultWidget::CloseResult()
{
	if (IsInViewport() == true)
	{
		if (GetGameInstance() != nullptr)
		{
			RemoveFromParent();
			RemoveDelegate();

			GameInstance->GameUnPause(FInputModeUIOnly());
		}
	}
}

void UResultWidget::SetResult()
{
	if (GetGameInstance() != nullptr)
	{
		float PlayTime = GameInstance->GetPlayTime();

		S = PlayTime + GetWorld()->GetTimeSeconds();
		GameInstance->SetPlayTime(S);
		M = S / 60;
		H = M / 60;
		S %= 60;

		FString h = FString::FromInt(H);
		if (H < 10) h = "0" + h;
		ResultTimeH->SetText(FText::FromString(h));
		FString m = FString::FromInt(M);
		if (M < 10) m = "0" + m;
		ResultTimeM->SetText(FText::FromString(m));
		FString s = FString::FromInt(S);
		if (S < 10) s = "0" + s;
		ResultTimeS->SetText(FText::FromString(s));

		uint32 FloorCount = GameInstance->GetFloorCount();
		ResultFloor->SetText(FText::FromString(FString::FromInt(FloorCount)));

		uint32 DefeatCount = GameInstance->GetDefeatEnemyCount();
		ResultDefeat->SetText(FText::FromString(FString::FromInt(DefeatCount)));
	}
}

void UResultWidget::ClickedBack()
{
	if (GetGameInstance() != nullptr)
	{
		CloseResult();
		GameInstance->SetOpenLevelType(EOpenLevelType::Title);
		GameInstance->BeginLoadingScreen(GameInstance->GetTitleLevel());
	}
}

void UResultWidget::ClickedSaveScore()
{
	if (GetGameInstance() != nullptr)
	{
		GameInstance->SetScore(GameInstance->GetPlayTime());
		GameInstance->SaveGame("Score", 5, true, true);
	}
}


void UResultWidget::AddDelegate()
{
	ButtonBack->OnClicked.AddDynamic(this, &UResultWidget::ClickedBack);
	ButtonSaveScore->OnClicked.AddDynamic(this, &UResultWidget::ClickedSaveScore);
}

void UResultWidget::RemoveDelegate()
{
	ButtonBack->OnClicked.RemoveDynamic(this, &UResultWidget::ClickedBack);
	ButtonSaveScore->OnClicked.RemoveDynamic(this, &UResultWidget::ClickedSaveScore);
}

URoguelikeGameInstance* UResultWidget::GetGameInstance()
{
	return GameInstance = URoguelikeGameInstance::GetGameInstance();
}