// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Public/RoguelikeGameMode.h"
#include "Public/RoguelikeGameInstance.h"

#include "Engine.h"
#include "Kismet/GameplayStatics.h"

ARoguelikeGameMode::ARoguelikeGameMode()
{
	GenerateExterior = CreateDefaultSubobject<AGenerateExterior>(TEXT("Exterior"));
}

void ARoguelikeGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = URoguelikeGameInstance::GetGameInstance();
	if (GameInstance != nullptr)
	{
		GameInstance->Init();
		GameInstance->SetActiveMode(EActiveMode::Dungeon);
		GameInstance->GameUnPause(FInputModeGameOnly());
		EOpenLevelType OpenType = GameInstance->GetOpenLevelType();
		if (OpenType == EOpenLevelType::Load)
		{
			GameInstance->LoadGame(GameInstance->GetSlotName(), GameInstance->GetSlotIndex());
		}
		GenerateExterior->GenerateExterior();
	}
}

/////////////////////////////////////////////////////////
// 

ARoguelikeGameMode* ARoguelikeGameMode::GetGameMode()
{
	AGameModeBase* tmp = UGameplayStatics::GetGameMode(GEngine->GameViewport->GetWorld());
	if (tmp == nullptr) return nullptr;

	return Cast<ARoguelikeGameMode>(tmp);
}
