// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Animation/MyAnimNotify_PlayerAbilityFinish.h"
#include "Public/Player/RoguelikeCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

#include "Engine.h"

void UMyAnimNotify_PlayerAbilityFinish::Notify(
	USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation
)
{
	if (InMeshComp != nullptr)
	{
		MeshComp = InMeshComp;
		Player = Cast<ARoguelikeCharacter>(InMeshComp->GetOwner());
		AnimInstance = MeshComp->GetAnimInstance();

		world = InMeshComp->GetWorld();
		if (world != nullptr)
		{
			//if (world->IsPlayInEditor() == false) return;
		}
	}

	if (Animation != nullptr)
	{
		AnimName = Animation->GetName();
		if (AnimName == "Aurora_Ability_R")
		{
			Player->Ability_R_Deactivate();
		}
		Player->SetPlayerState(EPlayerState::Idle);
	}
}