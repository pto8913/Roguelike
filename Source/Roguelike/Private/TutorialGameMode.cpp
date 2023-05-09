// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/TutorialGameMode.h"

#include "Engine.h"
#include "Kismet/GameplayStatics.h"

ATutorialGameMode* ATutorialGameMode::GetGameMode()
{
	AGameModeBase* tmp = UGameplayStatics::GetGameMode(GEngine->GameViewport->GetWorld());
	if (tmp == nullptr) return nullptr;

	return Cast<ATutorialGameMode>(tmp);
}

