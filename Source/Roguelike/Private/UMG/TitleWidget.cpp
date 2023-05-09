// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/UMG/TitleWidget.h"
#include "Public/UMG/LoadingWidget.h"
#include "Public/UMG/OptionWidget.h"
#include "Public/UMG/SelectDataWidget.h"

#include "Public/Player/RoguelikeCharacter.h"

#include "Public/RoguelikeGameInstance.h"
#include "Public/RoguelikeGameMode.h"

#include "Public/Save/RoguelikeSaveGame.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine.h"

void UTitleWidget::OpenTitle()
{
	if (!IsInViewport())
	{
		if (GetGameInstance() != nullptr)
		{
			AddToViewport();
			AddDelegate();

			GameInstance->Init();
			GameInstance->SetMouseCursor(true);
			GameInstance->SetInputMode(FInputModeUIOnly());
			GameInstance->LoadGame("Options", 6, false, true);

			PlayAnimation(FadeIn);
		}
	}
}

void UTitleWidget::CloseTitle()
{
	if (IsInViewport() == true)
	{
		if (GetGameInstance() != nullptr)
		{
			RemoveFromParent();
			RemoveDelegate();
			GameInstance->GameUnPause(FInputModeGameOnly());
		}
	}
}

void UTitleWidget::ClickedStart()
{
	if (GetGameInstance() != nullptr)
	{
		CloseTitle();
		
		GameInstance->SetDungeonInfos(FDungeonInfos::FDungeonInfos());
		GameInstance->SetOpenLevelType(EOpenLevelType::Main);
		GameInstance->BeginLoadingScreen(GameInstance->GetMainLevel());
	}
}

void UTitleWidget::ClickedTutorial()
{
	if (GetGameInstance() != nullptr)
	{
		GameInstance->BeginLoadingScreen(GameInstance->GetTutorialLevel());
	}
}

void UTitleWidget::ClickedOption()
{
	if (GetGameInstance() != nullptr)
	{
		OptionUI = GameInstance->GetOptionUI();
		if (OptionUI != nullptr)
		{
			OptionUI->SetOpenFromTitle(true);
			OptionUI->OpenOption();
		}
	}
}

void UTitleWidget::ClickedLoad()
{
	if (GetGameInstance() != nullptr)
	{
		SelectDataUI = GameInstance->GetSelectDataUI();
		if (SelectDataUI != nullptr)
		{
			SelectDataUI->OpenSelectData();
		}
	}
}

void UTitleWidget::ClickedExit()
{
	if (GetGameInstance() != nullptr)
	{
		GameInstance->SaveGame("Options", 6, false, false, true);

		UKismetSystemLibrary::QuitGame(
			GetWorld(), GameInstance->GetPlayerController(),
			EQuitPreference::Quit, false
		);
	}
}

void UTitleWidget::ClickedScoreBoard()
{
	if (GetGameInstance() != nullptr)
	{
		GameInstance->LoadGame("Score", 5, true);
		GameInstance->SetOpenLevelType(EOpenLevelType::Score);
		GameInstance->BeginLoadingScreen(GameInstance->GetScoreLevel());
	}
}

///////////////////////////////////////////////////////////////////////
// Templates

void UTitleWidget::AddDelegate()
{
	ButtonLoad->OnClicked.AddDynamic(this, &UTitleWidget::ClickedLoad);
	ButtonStart->OnClicked.AddDynamic(this, &UTitleWidget::ClickedStart);
	ButtonTutorial->OnClicked.AddDynamic(this, &UTitleWidget::ClickedTutorial);
	ButtonOption->OnClicked.AddDynamic(this, &UTitleWidget::ClickedOption);
	ButtonExit->OnClicked.AddDynamic(this, &UTitleWidget::ClickedExit);
	ButtonScoreBoard->OnClicked.AddDynamic(this, &UTitleWidget::ClickedScoreBoard);
}

void UTitleWidget::RemoveDelegate()
{
	ButtonLoad->OnClicked.RemoveDynamic(this, &UTitleWidget::ClickedLoad);
	ButtonStart->OnClicked.RemoveDynamic(this, &UTitleWidget::ClickedStart);
	ButtonTutorial->OnClicked.RemoveDynamic(this, &UTitleWidget::ClickedTutorial);
	ButtonOption->OnClicked.RemoveDynamic(this, &UTitleWidget::ClickedOption);
	ButtonExit->OnClicked.RemoveDynamic(this, &UTitleWidget::ClickedExit);
	ButtonScoreBoard->OnClicked.RemoveDynamic(this, &UTitleWidget::ClickedScoreBoard);
}

URoguelikeGameInstance* UTitleWidget::GetGameInstance()
{
	return GameInstance = URoguelikeGameInstance::GetGameInstance();
}
