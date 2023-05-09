// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "ResultWidget.generated.h"

class UTextBlock;
class UButton;
class URoguelikeGameInstance;

UCLASS()
class ROGUELIKE_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;
	URoguelikeGameInstance* GetGameInstance();

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ResultTimeH = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ResultTimeM = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ResultTimeS = nullptr;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ResultFloor = nullptr;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ResultDefeat = nullptr;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonBack = nullptr;
	UFUNCTION()
		void ClickedBack();

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonSaveScore = nullptr;
	/* Save Score ( SlotName = "Score", SlotIndex = 5 ) */
	UFUNCTION()
		void ClickedSaveScore();

	UPROPERTY(meta = (BindWidgetAnim))
		UWidgetAnimation* FadeIn;
	void SetResult();

	void AddDelegate();
	void RemoveDelegate();

	UPROPERTY()
		uint16 H = 0;

	UPROPERTY()
		uint16 M = 0;

	UPROPERTY()
		uint16 S = 0;
public:
	void OpenResult();
	void CloseResult();
};
