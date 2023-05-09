// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Button.h"

#include "TitleWidget.generated.h"

class ARoguelikeCharacter;
class UWidgetAnimation;
class UOptionWidget;
class ULoadingWidget;
class USelectDataWidget;
class URoguelikeGameInstance;

UCLASS()
class ROGUELIKE_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
		APlayerController* Controller = nullptr;

	UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;
	URoguelikeGameInstance* GetGameInstance();

	UPROPERTY()
		ARoguelikeCharacter* Player = nullptr;

	UPROPERTY()
		UOptionWidget* OptionUI = nullptr;

	UPROPERTY()
		ULoadingWidget* LoadUI = nullptr;

	UPROPERTY()
		USelectDataWidget* SelectDataUI = nullptr;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonLoad = nullptr;
	/* Open Select data ui. */
	UFUNCTION()
		void ClickedLoad();

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonStart = nullptr;
	UFUNCTION()
		void ClickedStart();

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonTutorial = nullptr;
	UFUNCTION()
		void ClickedTutorial();

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonOption = nullptr;
	UFUNCTION()
		void ClickedOption();

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonExit = nullptr;
	UFUNCTION()
		void ClickedExit();

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonScoreBoard = nullptr;
	/* Load score data and open score level */
	UFUNCTION()
		void ClickedScoreBoard();

	UPROPERTY(meta = (BindWidgetAnim))
		UWidgetAnimation* FadeIn;

	void AddDelegate();
	void RemoveDelegate();

public:
	UFUNCTION(BlueprintCallable)
		void OpenTitle();

	
	void CloseTitle();
};
