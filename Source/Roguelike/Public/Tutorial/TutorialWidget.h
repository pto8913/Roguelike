// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialWidget.generated.h"

class UTextBlock;
class UButton;
class URoguelikeGameInstance;

struct FTutorialText
{
	FTutorialText(
		const FString& f, const FString& g, const FString& h, const FString& i, const FString& j
	) : en_1(f), en_2(g), en_3(h), en_4(i), en_5(j) {}

	FString en_1, en_2, en_3, en_4, en_5;
};

UCLASS()
class ROGUELIKE_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* En_line1 = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* En_line2 = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* En_line3 = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* En_line4 = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* En_line5 = nullptr;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonBack = nullptr;
	UFUNCTION()
		void ClickedBack();

	UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;
	URoguelikeGameInstance* GetGameInstance();

	void AddDelegate();
	void RemoveDelegate();

	void Init();
	void SetText(uint8 Idx);

	TArray<FTutorialText> Texts;
public:
	void OpenTutorialWidget(uint8 Idx);
};
