// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TutorialGameMode.generated.h"

UCLASS()
class ROGUELIKE_API ATutorialGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		uint8 NotifyCount = 0;
public:
	static ATutorialGameMode* GetGameMode();

	void AddNotifyCount() { ++NotifyCount; }
	uint8 GetNotifyCount() const { return NotifyCount; }
};
