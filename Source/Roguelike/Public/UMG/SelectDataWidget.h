// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectDataWidget.generated.h"

class UTextBlock;
class UButton;
class URoguelikeGameInstance;
class URoguelikeSaveGame;

UCLASS()
class ROGUELIKE_API USelectDataWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;
	URoguelikeGameInstance* GetGameInstance();

	UPROPERTY()
		URoguelikeSaveGame* SaveGameInstance = nullptr;

	//////////////////////////////
	// Slot1
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Slot1Date = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Slot1H = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Slot1M = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Slot1S = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Slot1Floor = nullptr;
	UPROPERTY(meta = (BindWidget))
		UButton* ButtonLoadSlot1 = nullptr;
	UFUNCTION()
		void ClickedSlot1();

	//////////////////////////////
	// Slot2
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Slot2Date = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Slot2H = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Slot2M = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Slot2S = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Slot2Floor = nullptr;
	UPROPERTY(meta = (BindWidget))
		UButton* ButtonLoadSlot2 = nullptr;
	UFUNCTION()
		void ClickedSlot2();

	//////////////////////////////
	// Slot3
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Slot3Date = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Slot3H = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Slot3M = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Slot3S = nullptr;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Slot3Floor = nullptr;
	UPROPERTY(meta = (BindWidget))
		UButton* ButtonLoadSlot3 = nullptr;
	UFUNCTION()
		void ClickedSlot3();

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonBack;

	/*
		Set SlotName and SlotIndex and open loading screen.
		Actually Load data is execute in each game modes.
		Look RoguelikeGameInstance.h about SlotName and SlotIndex 
	*/
	UFUNCTION()
		void Load(const FString SlotName, const uint16 SlotIndex);

	/* 
		Before call Load, for check exists SlotName and SlotIndex in sav file
		if not exists return false. exists return true
	*/
	bool PreLoadCheck(const FString SlotName, const uint16 SlotIndex);

	void SetSelectData();
	/* 
		If exists SlotName and SlotIndex is in sav file, 
		then set text PlayTime, Floor, Date 
	*/
	void SetSelectData(
		const uint8 Idx, UTextBlock* Date, UTextBlock* Floor,
		UTextBlock* h, UTextBlock* m, UTextBlock* s
	);
	/* Get Hours, Minutes, Seconds from PlayTime */
	void GetTimeHMS(FText& H, FText& M, FText& S);

	void AddDelegate();
	void RemoveDelegate();

public:
	void OpenSelectData();
	UFUNCTION()
		void CloseSelectData();
};
