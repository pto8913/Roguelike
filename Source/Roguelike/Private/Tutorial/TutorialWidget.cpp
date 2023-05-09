// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Tutorial/TutorialWidget.h"
#include "Public/RoguelikeGameInstance.h"

#include "Button.h"
#include "TextBlock.h"

void UTutorialWidget::OpenTutorialWidget(uint8 Idx)
{
	if (!IsInViewport())
	{
		if (GetGameInstance() != nullptr)
		{
			AddToViewport();
			AddDelegate();
			GameInstance->GamePause(FInputModeUIOnly());
			Init();
			SetText(Idx);
		}
	}
}

void UTutorialWidget::Init()
{
	Texts =
	{
		FTutorialText(
			"Hi, all. this is the tutorial",
			"enjoy!",
			"At first. How to control.",
			"You can move W A S D key.try it!",
			"Jump space key."
		),

		FTutorialText(
			"Are you familiar with the control?",
			"Next. press Z key.",
			"Actually, everything is written there.",
			"You can also save",
			" "
		),

		FTutorialText(
			"Look! There are skeletons in front of you!",
			"They are your enemy.",
			"Each has its own characteristics.",
			"Check it out by playing it.",
			"Well, they doesn't work in tutorial. lol"
		),

		FTutorialText(
			"oh, there are the portal..",
			"The right one is you can go to the next floor.",
			"The left one is... I don't know",
			" ",
			"Just look at it"
		),

		FTutorialText(
			"Tutorial is finished!",
			"Thank you!",
			"You can go back to the title through that portal.",
			"That works fine. lol ",
			"Bye"
		)
	};
}

void UTutorialWidget::ClickedBack()
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

void UTutorialWidget::SetText(uint8 Idx)
{
	En_line1->SetText(FText::FromString(Texts[Idx].en_1));
	En_line2->SetText(FText::FromString(Texts[Idx].en_2));
	En_line3->SetText(FText::FromString(Texts[Idx].en_3));
	En_line4->SetText(FText::FromString(Texts[Idx].en_4));
	En_line5->SetText(FText::FromString(Texts[Idx].en_5));
}

void UTutorialWidget::AddDelegate()
{
	ButtonBack->OnClicked.AddDynamic(this, &UTutorialWidget::ClickedBack);
}

void UTutorialWidget::RemoveDelegate()
{
	ButtonBack->OnClicked.RemoveDynamic(this, &UTutorialWidget::ClickedBack);
}

URoguelikeGameInstance* UTutorialWidget::GetGameInstance()
{
	return GameInstance = URoguelikeGameInstance::GetGameInstance();
}