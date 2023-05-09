// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/ScoreGameMode.h"
#include "Public/RoguelikeGameInstance.h"
#include "Public/UMG/ScoreWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Engine.h"


void AScoreGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = URoguelikeGameInstance::GetGameInstance();
	if (GameInstance != nullptr)
	{
		GameInstance->SetInputMode(FInputModeUIOnly());
		ScoreUI = GameInstance->GetScoreUI();
		if (ScoreUI != nullptr)
		{
			ScoreUI->OpenScore();
		}
	}
}


AScoreGameMode* AScoreGameMode::GetGameMode()
{
	AGameModeBase* tmp = UGameplayStatics::GetGameMode(GEngine->GameViewport->GetWorld());
	if (tmp == nullptr) return nullptr;

	return Cast<AScoreGameMode>(tmp);
}
