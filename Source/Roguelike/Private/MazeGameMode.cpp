// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/MazeGameMode.h"
#include "Public/Dungeon/GenerateExterior.h"
#include "Public/RoguelikeGameInstance.h"

#include "Engine.h"
#include "Kismet/GameplayStatics.h"


AMazeGameMode::AMazeGameMode()
{
	GenerateExterior = CreateDefaultSubobject<AGenerateExterior>(TEXT("Exterior"));
}

void AMazeGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = GameInstance = URoguelikeGameInstance::GetGameInstance();
	if (GameInstance != nullptr)
	{
		GameInstance->Init();
		GameInstance->SetActiveMode(EActiveMode::Maze);
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

AMazeGameMode* AMazeGameMode::GetGameMode()
{
	AGameModeBase* tmp = UGameplayStatics::GetGameMode(GEngine->GameViewport->GetWorld());
	if (tmp == nullptr) return nullptr;

	return Cast<AMazeGameMode>(tmp);
}
