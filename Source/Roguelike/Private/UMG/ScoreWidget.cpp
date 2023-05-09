// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/UMG/ScoreWidget.h"
#include "Public/RoguelikeGameInstance.h"

#include "Engine.h"
#include "Button.h"
#include "TextBlock.h"

void UScoreWidget::OpenScore()
{
	if (!IsInViewport())
	{
		GameInstance = URoguelikeGameInstance::GetGameInstance();

		if (GameInstance != nullptr)
		{
			AddToViewport();
			AddDelegate();

			GameInstance->SetInputMode(FInputModeUIOnly());
			GameInstance->SetMouseCursor(true);
			Score = GameInstance->GetScore();
			Default = Score;

			SetScore();
		}
	}
}

void UScoreWidget::SetScore()
{
	SetScore(0, DefeatEnemy1, Floor1, PlayTime1H, PlayTime1M, PlayTime1S);
	SetScore(1, DefeatEnemy2, Floor2, PlayTime2H, PlayTime2M, PlayTime2S);
	SetScore(2, DefeatEnemy3, Floor3, PlayTime3H, PlayTime3M, PlayTime3S);
}

void UScoreWidget::SetScore(
	const uint8 Idx,
	UTextBlock* DE,
	UTextBlock* F,
	UTextBlock* PTH, UTextBlock* PTM, UTextBlock* PTS
)
{
	if (Score.IsValidIndex(Idx) == true)
	{
		FScores tmp = Score[Idx];
		DE->SetText(FText::FromString(FString::FromInt(tmp.DefeatEnemy)));
		F->SetText(FText::FromString(FString::FromInt(tmp.Floor)));

		FText H = FText(), M = FText(), S = FText();

		GetTimeHMS(tmp.PlayTime, H, M, S);
		PTH->SetText(H);
		PTM->SetText(M);
		PTS->SetText(S);
	}
}

void UScoreWidget::GetTimeHMS(const float In, FText& H, FText& M, FText& S)
{
	uint16 m = In / 60;
	FString mm = FString::FromInt(m);
	if (m < 10) mm = "0" + mm;

	uint16 h = m / 60;
	FString hh = FString::FromInt(h);
	if (h < 10) hh = "0" + hh;

	uint16 s = (uint16)In % 60;
	FString ss = FString::FromInt(s);
	if (s < 10) ss = "0" + ss;

	H = FText::FromString(hh);
	M = FText::FromString(mm);
	S = FText::FromString(ss);
}

void UScoreWidget::ClickedBack()
{
	if (IsInViewport())
	{
		RemoveDelegate();
		GameInstance->BeginLoadingScreen(GameInstance->GetTitleLevel());
	}
}

void UScoreWidget::SortDefeatEnemy()
{
	++DE_ClickedCount;
	DE_ClickedCount %= 3;
	Sort();
	if (DE_ClickedCount == 0)
	{
		DE_SortType->SetText(FText::FromString(" "));
	}
	else if (DE_ClickedCount == 1)
	{
		DE_SortType->SetText(FText::FromString("Asc"));
	}
	else if (DE_ClickedCount == 2)
	{
		DE_SortType->SetText(FText::FromString("Desc"));
	}
}

void UScoreWidget::SortFloor()
{
	++F_ClickedCount;
	F_ClickedCount %= 3;
	Sort();
	if (F_ClickedCount == 0)
	{
		F_SortType->SetText(FText::FromString(" "));
	}
	else if (F_ClickedCount == 1)
	{
		F_SortType->SetText(FText::FromString("Asc"));
	}
	else if (F_ClickedCount == 2)
	{
		F_SortType->SetText(FText::FromString("Desc"));
	}
}

void UScoreWidget::SortPlayTime()
{
	++PT_ClickedCount;
	PT_ClickedCount %= 3;
	Sort();
	if (PT_ClickedCount == 0)
	{
		PT_SortType->SetText(FText::FromString(" "));
	}
	else if (PT_ClickedCount == 1)
	{
		PT_SortType->SetText(FText::FromString("Asc"));
	}
	else if (PT_ClickedCount == 2)
	{
		PT_SortType->SetText(FText::FromString("Desc"));
	}
}

void UScoreWidget::Sort()
{
	if (DE_ClickedCount == 0)
	{
		if (F_ClickedCount == 0)
		{
			if (PT_ClickedCount == 0)
			{
				Score = Default;
			}
			else if (PT_ClickedCount == 1)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.PlayTime < B.PlayTime;
					});
			}
			else if (PT_ClickedCount == 2)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.PlayTime > B.PlayTime;
					});
			}
		}
		else if (F_ClickedCount == 1)
		{
			// Asc F
			if (PT_ClickedCount == 0)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.Floor < B.Floor;
					});
			}
			else if (PT_ClickedCount == 1)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.Floor < B.Floor && A.PlayTime < B.Floor;
					});
			}
			else if (PT_ClickedCount == 2)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.Floor < B.Floor && A.PlayTime > B.Floor;
					});
			}
		}
		else if (F_ClickedCount == 2)
		{
			// Desc F
			if (PT_ClickedCount == 0)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.Floor > B.Floor;
					});
			}
			else if (PT_ClickedCount == 1)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.Floor > B.Floor&& A.PlayTime < B.Floor;
					});
			}
			else if (PT_ClickedCount == 2)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.Floor > B.Floor&& A.PlayTime > B.Floor;
					});
			}
		}
	}
	else if (DE_ClickedCount == 1)
	{
		// Asc DE
		if (F_ClickedCount == 0)
		{
			if (PT_ClickedCount == 0)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy < B.DefeatEnemy;
					});
			}
			else if (PT_ClickedCount == 1)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy < B.DefeatEnemy && A.PlayTime < B.PlayTime;
					});
			}
			else if (PT_ClickedCount == 2)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy < B.DefeatEnemy && A.PlayTime > B.PlayTime;
					});
			}
		}
		else if (F_ClickedCount == 1)
		{
			// Asc F
			if (PT_ClickedCount == 0)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy < B.DefeatEnemy && A.Floor < B.Floor;
					});
			}
			else if (PT_ClickedCount == 1)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy < B.DefeatEnemy && A.Floor < B.Floor && A.PlayTime < B.PlayTime;
					});
			}
			else if (PT_ClickedCount == 2)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy < B.DefeatEnemy && A.Floor < B.Floor && A.PlayTime > B.PlayTime;
					});
			}
		}
		else if (F_ClickedCount == 2)
		{
			// Desc F
			if (PT_ClickedCount == 0)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy < B.DefeatEnemy && A.Floor > B.Floor;
					});
			}
			else if (PT_ClickedCount == 1)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy < B.DefeatEnemy && A.Floor > B.Floor && A.PlayTime < B.PlayTime;
					});
			}
			else if (PT_ClickedCount == 2)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy < B.DefeatEnemy && A.Floor > B.Floor && A.PlayTime > B.PlayTime;
					});
			}
		}
	}
	else if (DE_ClickedCount == 2)
	{
		// Desc DE
		if (F_ClickedCount == 0)
		{
			if (PT_ClickedCount == 0)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy > B.DefeatEnemy;
					});
			}
			else if (PT_ClickedCount == 1)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy > B.DefeatEnemy&& A.PlayTime < B.PlayTime;
					});
			}
			else if (PT_ClickedCount == 2)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy > B.DefeatEnemy&& A.PlayTime > B.PlayTime;
					});
			}
		}
		else if (F_ClickedCount == 1)
		{
			// Asc F
			if (PT_ClickedCount == 0)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy > B.DefeatEnemy&& A.Floor < B.Floor;
					});
			}
			else if (PT_ClickedCount == 1)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy > B.DefeatEnemy&& A.Floor < B.Floor && A.PlayTime < B.PlayTime;
					});
			}
			else if (PT_ClickedCount == 2)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy > B.DefeatEnemy&& A.Floor < B.Floor && A.PlayTime > B.PlayTime;
					});
			}
		}
		else if (F_ClickedCount == 2)
		{
			// Desc F
			if (PT_ClickedCount == 0)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy > B.DefeatEnemy&& A.Floor > B.Floor;
					});
			}
			if (PT_ClickedCount == 1)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy > B.DefeatEnemy&& A.Floor > B.Floor&& A.PlayTime < B.PlayTime;
					});
			}
			else if (PT_ClickedCount == 2)
			{
				Score.Sort([](const FScores& A, const FScores& B) {
					return A.DefeatEnemy > B.DefeatEnemy&& A.Floor > B.Floor&& A.PlayTime > B.PlayTime;
					});
			}
		}
	}
	SetScore();
}

void UScoreWidget::AddDelegate()
{
	ButtonBack->OnClicked.AddDynamic(this, &UScoreWidget::ClickedBack);
	ButtonSortDefeatEnemy->OnClicked.AddDynamic(this, &UScoreWidget::SortDefeatEnemy);
	ButtonSortFloor->OnClicked.AddDynamic(this, &UScoreWidget::SortFloor);
	ButtonSortPlayTime->OnClicked.AddDynamic(this, &UScoreWidget::SortPlayTime);
}

void UScoreWidget::RemoveDelegate()
{
	ButtonBack->OnClicked.RemoveDynamic(this, &UScoreWidget::ClickedBack);
	ButtonSortDefeatEnemy->OnClicked.RemoveDynamic(this, &UScoreWidget::SortDefeatEnemy);
	ButtonSortFloor->OnClicked.RemoveDynamic(this, &UScoreWidget::SortFloor);
	ButtonSortPlayTime->OnClicked.RemoveDynamic(this, &UScoreWidget::SortPlayTime);
}

