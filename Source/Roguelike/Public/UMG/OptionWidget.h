// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ComboBoxString.h"
#include "SComboBox.h"
#include "SpinBox.h"
#include "CheckBox.h"
#include "TextBlock.h"
#include "Public/Templates/ptoGenerator.h"
#include "Public/Templates/ptoStructs.h"
#include "OptionWidget.generated.h"

class ARoguelikeCharacter;
class ARoguelikeGameMode;
class AMazeGameMode;
class UButton;
class URoguelikeGameInstance;

UCLASS()
class ROGUELIKE_API UOptionWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;
	URoguelikeGameInstance* GetGameInstance();

	UPROPERTY()
		AMazeGameMode* MazeGameMode = nullptr;
	UPROPERTY()
		ARoguelikeGameMode* RoguelikeGameMode = nullptr;

	UPROPERTY()
		ARoguelikeCharacter* Player = nullptr;

	/////////////////////////////////////
	// Save Game
	UPROPERTY(meta = (BindWidget))
		UButton* ButtonSave = nullptr;
	UFUNCTION()
		void ClickedSave();

	UPROPERTY(meta = (BindWidget))
		UComboBoxString* SaveSlotCombo = nullptr;

	/////////////////////////////////////
	// Close Option
	UPROPERTY(meta = (BindWidget))
		UButton* ButtonClose = nullptr;

	/////////////////////////////////////
	// Back Title
	UPROPERTY(meta = (BindWidget))
		UButton* ButtonBackTitle = nullptr;
	UFUNCTION()
		void ClickedBackTitle();

	/////////////////////////////////////
	// User Settings
	void InitUserSettings();
	UPROPERTY()
		FOptions Options;

	/* Graphic */
	/* this setting is title only */
	UPROPERTY(meta = (BindWidget))
		UComboBoxString* GraphicQualityCombo = nullptr;
	UFUNCTION()
		void GraphicSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	/* Camera */
	UPROPERTY(meta = (BindWidget))
		USpinBox* DistanceToCameraSpin = nullptr;
	UFUNCTION()
		void DistanceToCameraEndSlider(float InValue);
	UFUNCTION()
		void DistanceToCameraCommitted(float InValue, ETextCommit::Type CommitMethod);
	void ClampCameraArmLength(float& In);

	/////////////////////////////////////
	// 
	void AddDelegate();
	void RemoveDelegate();

	UPROPERTY()
		bool bIsOpenFromTitle = false;

public:
	void OpenOption();

	UFUNCTION()
		void CloseOption();

	void SetOpenFromTitle(bool bIsChecked) { bIsOpenFromTitle = bIsChecked; }
	bool GetOpenFromTitle() { return bIsOpenFromTitle; }
};
