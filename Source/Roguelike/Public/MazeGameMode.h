// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MazeGameMode.generated.h"

class URoguelikeGameInstance;
class AGenerateExterior;

UCLASS()
class ROGUELIKE_API AMazeGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;

	UPROPERTY()
		AGenerateExterior* GenerateExterior = nullptr;
public:
	AMazeGameMode();

	static AMazeGameMode* GetGameMode();

	virtual void BeginPlay() override;
};
