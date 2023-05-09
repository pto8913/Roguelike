// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/UMG/OptionWidget.h"
#include "Public/Player/RoguelikeCharacter.h"
#include "Public/RoguelikeGameInstance.h"
#include "Public/RoguelikeGameMode.h"
#include "Public/MazeGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Button.h"
#include "Engine.h"

void UOptionWidget::OpenOption()
{
	if (!IsInViewport())
	{
		MazeGameMode = AMazeGameMode::GetGameMode();
		RoguelikeGameMode = ARoguelikeGameMode::GetGameMode();
		if (GetGameInstance() != nullptr)
		{
			AddToViewport();
			AddDelegate();
			InitUserSettings();
			if (!bIsOpenFromTitle)
			{
				GameInstance->GamePause(FInputModeUIOnly());
				GraphicQualityCombo->SetIsEnabled(false);
			}
			if (MazeGameMode == nullptr && RoguelikeGameMode == nullptr)
			{
				ButtonSave->SetIsEnabled(false);
			}
		}
	}
}

void UOptionWidget::CloseOption()
{
	if (IsInViewport() == true)
	{
		if (GetGameInstance() != nullptr)
		{
			RemoveFromParent();
			RemoveDelegate();

			GameInstance->SetOptions(Options);
			GameInstance->SaveGame("Options", 6, false, false, true);

			if (!bIsOpenFromTitle)
			{
				GameInstance->GameUnPause(FInputModeGameOnly());
			}
			else
			{
				bIsOpenFromTitle = false;
			}
		}
		GraphicQualityCombo->SetIsEnabled(true);
		ButtonSave->SetIsEnabled(true);
	}
}

void UOptionWidget::ClickedBackTitle()
{
	if (IsInViewport() == true)
	{
		RemoveFromParent();
		RemoveDelegate();
		if (GetGameInstance() != nullptr)
		{
			GameInstance->SetOptions(Options);
			GameInstance->SaveGame("Options", 6, false, false, true);
		}
	}
	if (bIsOpenFromTitle == true)
	{
		bIsOpenFromTitle = false;
	}
	else
	{
		if (GetGameInstance() != nullptr)
		{
			GameInstance->SetOpenLevelType(EOpenLevelType::Title);
			GameInstance->BeginLoadingScreen(GameInstance->GetTitleLevel());
		}
	}
}

void UOptionWidget::ClickedSave()
{
	if (GetGameInstance() != nullptr)
	{
		if (SaveSlotCombo != nullptr)
		{
			FString SlotName = SaveSlotCombo->GetSelectedOption();
			uint16 SlotIndex = SaveSlotCombo->GetSelectedIndex();
			GameInstance->SaveGame(SlotName, SlotIndex);
		}
	}
}

/////////////////////////////////////////////////////
// user settings

void UOptionWidget::InitUserSettings()
{
	if (GetGameInstance() != nullptr)
	{
		Options = GameInstance->GetOptions();
		float CameraArmLength = Options.CameraArmLength;
		ClampCameraArmLength(CameraArmLength);
		DistanceToCameraSpin->SetValue(CameraArmLength);

		uint32 Idx = Options.GraphicIndex;
		GraphicQualityCombo->SetSelectedIndex(Idx);
	}
}

void UOptionWidget::GraphicSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem == "High")
	{
		Options.GraphicType = EGraphicType::High;
	}
	else if (SelectedItem == "Mid")
	{
		Options.GraphicType = EGraphicType::Mid;
	}
	else if (SelectedItem == "Low")
	{
		Options.GraphicType = EGraphicType::Low;
	}
	Options.GraphicIndex = GraphicQualityCombo->GetSelectedIndex();
}

void UOptionWidget::DistanceToCameraEndSlider(float InValue)
{
	ClampCameraArmLength(InValue);
	DistanceToCameraSpin->SetValue(InValue);
	if (GetGameInstance() != nullptr)
	{
		GameInstance->GetPlayer()->SetCameraArmLength(InValue);
	}
	Options.CameraArmLength = InValue;
}

void UOptionWidget::DistanceToCameraCommitted(float InValue, ETextCommit::Type CommitMethod)
{
	ClampCameraArmLength(InValue);
	DistanceToCameraSpin->SetValue(InValue);
	if (GetGameInstance() != nullptr)
	{
		GameInstance->GetPlayer()->SetCameraArmLength(InValue);
	}
	Options.CameraArmLength = InValue;
}

void UOptionWidget::ClampCameraArmLength(float& In)
{
	In = FMath::Clamp(In, 300.f, 1000.f);
}

//////////////////////////////////////////////////////////////////
//

void UOptionWidget::AddDelegate()
{
	ButtonClose->OnClicked.AddDynamic(this, &UOptionWidget::CloseOption);
	ButtonBackTitle->OnClicked.AddDynamic(this, &UOptionWidget::ClickedBackTitle);
	ButtonSave->OnClicked.AddDynamic(this, &UOptionWidget::ClickedSave);
	if (bIsOpenFromTitle == true)
	{
		GraphicQualityCombo->OnSelectionChanged.AddDynamic(this, &UOptionWidget::GraphicSelectionChanged);
	}

	DistanceToCameraSpin->OnEndSliderMovement.AddDynamic(this, &UOptionWidget::DistanceToCameraEndSlider);
	DistanceToCameraSpin->OnValueCommitted.AddDynamic(this, &UOptionWidget::DistanceToCameraCommitted);
}


void UOptionWidget::RemoveDelegate()
{
	ButtonClose->OnClicked.RemoveDynamic(this, &UOptionWidget::CloseOption);
	ButtonBackTitle->OnClicked.RemoveDynamic(this, &UOptionWidget::ClickedBackTitle);
	ButtonSave->OnClicked.RemoveDynamic(this, &UOptionWidget::ClickedSave);
	if (bIsOpenFromTitle == true)
	{
		GraphicQualityCombo->OnSelectionChanged.RemoveDynamic(this, &UOptionWidget::GraphicSelectionChanged);
	}

	DistanceToCameraSpin->OnEndSliderMovement.RemoveDynamic(this, &UOptionWidget::DistanceToCameraEndSlider);
	DistanceToCameraSpin->OnValueCommitted.RemoveDynamic(this, &UOptionWidget::DistanceToCameraCommitted);
}


URoguelikeGameInstance* UOptionWidget::GetGameInstance()
{
	return GameInstance = URoguelikeGameInstance::GetGameInstance();
}