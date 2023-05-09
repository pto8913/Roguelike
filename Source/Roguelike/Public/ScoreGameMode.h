// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ScoreGameMode.generated.h"

class URoguelikeGameInstance;
class UScoreWidget;

UCLASS()
class ROGUELIKE_API AScoreGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;

	UPROPERTY()
		UScoreWidget* ScoreUI = nullptr;
public:
	static AScoreGameMode* GetGameMode();

	virtual void BeginPlay() override;

};
