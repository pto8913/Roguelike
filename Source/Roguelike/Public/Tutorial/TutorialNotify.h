// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/Tutorial/TutorialWidget.h"
#include "TutorialNotify.generated.h"

class UBoxComponent;
class ATutorialGameMode;
class URoguelikeGameInstance;

/*
	for open tutorial message ui
*/

UCLASS()
class ROGUELIKE_API ATutorialNotify : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Category = pto, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* NotifyCollision = nullptr;

	UPROPERTY()
		ATutorialGameMode* GameMode = nullptr;

	UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;

	UPROPERTY()
		UTutorialWidget* TutorialUI = nullptr;
public:
	// Sets default values for this actor's properties
	ATutorialNotify();

	UFUNCTION()
		void OnComponentOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);
};
