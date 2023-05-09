// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Tutorial/TutorialNotify.h"
#include "Public/TutorialGameMode.h"
#include "Public/RoguelikeGameInstance.h"

#include "Components/BoxComponent.h"

// Sets default values
ATutorialNotify::ATutorialNotify()
{
	NotifyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Notify Collision Comp"));
	NotifyCollision->SetRelativeScale3D(FVector(10.f, 0.1f, 5.f));
	NotifyCollision->OnComponentBeginOverlap.AddDynamic(this, &ATutorialNotify::OnComponentOverlap);
}

void ATutorialNotify::OnComponentOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (OtherActor->ActorHasTag("Player") == true || OtherComp->ComponentHasTag("Player") == true)
	{
		GameMode = ATutorialGameMode::GetGameMode();
		if (GameMode != nullptr)
		{
			GameInstance = URoguelikeGameInstance::GetGameInstance();
			if (GameInstance != nullptr)
			{
				Destroy();
				uint8 UIIdx = GameMode->GetNotifyCount();
				TutorialUI = GameInstance->GetTutorialUI();
				if (TutorialUI != nullptr)
				{
					TutorialUI->OpenTutorialWidget(UIIdx);
				}
				GameMode->AddNotifyCount();
			}
		}
	}
}