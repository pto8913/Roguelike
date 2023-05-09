// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/Templates/ptoStructs.h"
#include "ScoreWidget.generated.h"

class UButton;
class UTextBlock;
class URoguelikeGameInstance;

UCLASS()
class ROGUELIKE_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	/*
		DefeatEnemy
		Floor
		PlayTime
	*/
	UPROPERTY(meta = (BindWidget))
		UTextBlock* DefeatEnemy1 = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Floor1 = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayTime1H = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayTime1M = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayTime1S = nullptr;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* DefeatEnemy2 = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Floor2 = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayTime2H = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayTime2M = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayTime2S = nullptr;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* DefeatEnemy3 = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Floor3 = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayTime3H = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayTime3M = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayTime3S = nullptr;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonBack = nullptr;
	UFUNCTION()
		void ClickedBack();

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonSortDefeatEnemy = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* DE_SortType = nullptr;
	UFUNCTION()
		void SortDefeatEnemy();
	UPROPERTY()
		uint8 DE_ClickedCount = 0;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonSortFloor = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* F_SortType = nullptr;
	UFUNCTION()
		void SortFloor();
	UPROPERTY()
		uint8 F_ClickedCount = 0;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonSortPlayTime = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PT_SortType = nullptr;
	UFUNCTION()
		void SortPlayTime();
	UPROPERTY()
		uint8 PT_ClickedCount = 0;

	UFUNCTION()
		void Sort();

	void GetTimeHMS(const float In, FText& H, FText& M, FText& S);

	UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;

	TArray<FScores> Score;
	TArray<FScores> Default;

	UFUNCTION()
		void SetScore();

	void SetScore(
		const uint8 Idx,
		UTextBlock* DE,
		UTextBlock* F,
		UTextBlock* PTH, UTextBlock* PTM, UTextBlock* PTS
	);

	void AddDelegate();
	void RemoveDelegate();
public:
	UFUNCTION(BlueprintCallable)
		void OpenScore();
};
