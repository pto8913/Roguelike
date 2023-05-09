// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Animation/MyAnimNotify_Death.h"
#include "Public/RoguelikeGameInstance.h"
#include "Public/UMG/ResultWidget.h"

void UMyAnimNotify_Death::Notify(
	USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation
)
{
	URoguelikeGameInstance* GameInstance = URoguelikeGameInstance::GetGameInstance();

	if (InMeshComp != nullptr) {
		world = InMeshComp->GetWorld();
	}

	if (world != nullptr)
	{
		//if (world->IsPlayInEditor() == false) return;

		InMeshComp->GetOwner()->Destroy();
		if (Animation != nullptr)
		{
			FString AnimName = Animation->GetName();
			if (AnimName == "Aurora_DeathMontage")
			{
				if (GameInstance != nullptr)
				{
					UResultWidget* ResultUI = GameInstance->GetResultUI();
					if (ResultUI != nullptr)
					{
						ResultUI->OpenResult();
					}
				}
			}
		}
	}
}