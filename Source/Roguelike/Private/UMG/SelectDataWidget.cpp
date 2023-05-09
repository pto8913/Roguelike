// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/UMG/SelectDataWidget.h"

#include "Button.h"
#include "TextBlock.h"
#include "Kismet/GameplayStatics.h"

#include "Public/Save/RoguelikeSaveGame.h"
#include "Public/RoguelikeGameInstance.h"

void USelectDataWidget::OpenSelectData()
{
	if (!IsInViewport())
	{
		AddToViewport();
		AddDelegate();
		SetSelectData();
	}
}

void USelectDataWidget::CloseSelectData()
{
	if (IsInViewport())
	{
		RemoveFromParent();
		RemoveDelegate();
	}
}

void USelectDataWidget::SetSelectData()
{
	SetSelectData(0, Slot1Date, Slot1Floor, Slot1H, Slot1M, Slot1S);
	SetSelectData(1, Slot2Date, Slot2Floor, Slot2H, Slot2M, Slot2S);
	SetSelectData(2, Slot3Date, Slot3Floor, Slot3H, Slot3M, Slot3S);
}

void USelectDataWidget::SetSelectData(
	const uint8 Idx, UTextBlock* Date, UTextBlock* Floor,
	UTextBlock* h, UTextBlock* m, UTextBlock* s
)
{
	TArray<FString> tmp = { "Slot1", "Slot2", "Slot3" };
	SaveGameInstance = Cast<URoguelikeSaveGame>(
		UGameplayStatics::LoadGameFromSlot(tmp[Idx], Idx)
	);

	if (SaveGameInstance == nullptr)
	{
		Date->SetText(FText::FromString("None"));
		Floor->SetText(FText::FromString("0 F"));
	}
	else
	{
		Date->SetText(FText::FromString(SaveGameInstance->TimeStamp.ToString()));
		FString floor = FString::FromInt(SaveGameInstance->DungeonInfos.FloorCount);
		FString Mode = "In ";
		if (SaveGameInstance->DungeonInfos.ActiveMode == EActiveMode::Dungeon) Mode += "Dungeon ";
		else if (SaveGameInstance->DungeonInfos.ActiveMode == EActiveMode::Maze) Mode += "Maze ";
		Floor->SetText(FText::FromString(Mode + floor + " F"));

		FText H = FText(), M = FText(), S = FText();
		GetTimeHMS(H, M, S);
		h->SetText(H);
		m->SetText(M);
		s->SetText(S);
	}
}

void USelectDataWidget::GetTimeHMS(FText& H, FText& M, FText& S)
{
	float PlayTime = SaveGameInstance->DungeonInfos.PlayTime;
	uint16 m = PlayTime / 60;
	FString mm = FString::FromInt(m);
	if (m < 10) mm = "0" + mm;

	uint16 h = m / 60;
	FString hh = FString::FromInt(h);
	if (h < 10) hh = "0" + hh;

	uint16 s = (uint16)PlayTime % 60;
	FString ss = FString::FromInt(s);
	if (s < 10) ss = "0" + ss;

	H = FText::FromString(hh);
	M = FText::FromString(mm);
	S = FText::FromString(ss);
}

void USelectDataWidget::Load(const FString SlotName, const uint16 SlotIndex)
{
	if (PreLoadCheck(SlotName, SlotIndex) == false) return;

	if (GetGameInstance() != nullptr)
	{
		GameInstance->SetOpenLevelType(EOpenLevelType::Load);
		GameInstance->SetSlotName(SlotName);
		GameInstance->SetSlotIndex(SlotIndex);
		EActiveMode ActiveMode = SaveGameInstance->DungeonInfos.ActiveMode;
		if (ActiveMode == EActiveMode::Dungeon)
		{
			GameInstance->BeginLoadingScreen(GameInstance->GetMainLevel());
		}
		else if (ActiveMode == EActiveMode::Maze)
		{
			GameInstance->BeginLoadingScreen(GameInstance->GetMazeLevel());
		}
	}
}

bool USelectDataWidget::PreLoadCheck(const FString SlotName, const uint16 SlotIndex)
{
	SaveGameInstance = Cast<URoguelikeSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex));
	if (SaveGameInstance == nullptr)
	{
		return false;
	}
	return true;
}

void USelectDataWidget::ClickedSlot1()
{
	Load("Slot1", 0);
}

void USelectDataWidget::ClickedSlot2()
{
	Load("Slot2", 1);
}

void USelectDataWidget::ClickedSlot3()
{
	Load("Slot3", 2);
}

void USelectDataWidget::AddDelegate()
{
	ButtonLoadSlot1->OnClicked.AddDynamic(this, &USelectDataWidget::ClickedSlot1);
	ButtonLoadSlot2->OnClicked.AddDynamic(this, &USelectDataWidget::ClickedSlot2);
	ButtonLoadSlot3->OnClicked.AddDynamic(this, &USelectDataWidget::ClickedSlot3);
	ButtonBack->OnClicked.AddDynamic(this, &USelectDataWidget::CloseSelectData);
}

void USelectDataWidget::RemoveDelegate()
{
	ButtonLoadSlot1->OnClicked.RemoveDynamic(this, &USelectDataWidget::ClickedSlot1);
	ButtonLoadSlot2->OnClicked.RemoveDynamic(this, &USelectDataWidget::ClickedSlot2);
	ButtonLoadSlot3->OnClicked.RemoveDynamic(this, &USelectDataWidget::ClickedSlot3);
	ButtonBack->OnClicked.RemoveDynamic(this, &USelectDataWidget::CloseSelectData);
}

URoguelikeGameInstance* USelectDataWidget::GetGameInstance()
{
	return GameInstance = URoguelikeGameInstance::GetGameInstance();
}

