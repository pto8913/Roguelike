// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextBlock.h"
#include "Image.h"
#include "NotifierWidget.generated.h"

class URoguelikeGameInstance;
class UButton;

/*
	When portal is spawned. appear notifier
*/
UCLASS()
class ROGUELIKE_API UNotifierWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
		UButton* ButtonClose = nullptr;

	UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;
	URoguelikeGameInstance* GetGameInstance();

	void AddDelegate();
	void RemoveDelegate();

public:
	UFUNCTION()
		void CloseNotifier();
	void OpenNotifier();
	
};
