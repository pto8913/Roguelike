// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProgressBar.h"
#include "PlayerWidget.generated.h"

/**
 *
 */
UCLASS()
class ROGUELIKE_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
		UProgressBar* PlayerHealth = nullptr;

public:
	void OpenPlayerUI(float NewHealth);
	void ClosePlayerUI();
	void UpdateHealth(float NewHealth);
};
