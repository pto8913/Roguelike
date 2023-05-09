// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyWidget.generated.h"

class URoguelikeGmaeInstance;
class UProgressBar;

UCLASS()
class ROGUELIKE_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
		UProgressBar* EnemyHealth = nullptr;

	UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;

	virtual void PreConstruct();
public:
	void OpenEnemyUI(float NewHealth);
	void CloseEnemyUI();
	
	void UpdateHealth(float NewHealth);
	void SetUpdateHPGaugePosition(ACharacter* ParentCharacter);
};
