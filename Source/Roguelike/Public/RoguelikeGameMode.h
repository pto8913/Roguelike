// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Public/Dungeon/GenerateExterior.h"
#include "Public/Templates/ptoGenerator.h"
#include "RoguelikeGameMode.generated.h"

class URoguelikeGameInstance;

UCLASS(minimalapi)
class ARoguelikeGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		AGenerateExterior* GenerateExterior = nullptr;

	UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;
public:
	ARoguelikeGameMode();

	static ARoguelikeGameMode* GetGameMode();

	virtual void BeginPlay() override;
};