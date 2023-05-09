// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Animation/MyAnimNotify_PlayerAbility.h"
#include "Public/Player/RoguelikeCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimInstance.h"

#include "Engine.h"

void UMyAnimNotify_PlayerAbility::Notify(
	USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation
)
{
	if (InMeshComp != nullptr)
	{
		MeshComp = InMeshComp;
		Player = Cast<ARoguelikeCharacter>(MeshComp->GetOwner());
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

		CurrentLocation = Player->GetActorLocation();
		CurrentRotation = Player->GetActorRotation();

		if (AnimName == "Aurora_Ability_E")
		{
			Ability_E();
		}
		else if (AnimName == "Aurora_Ability_R")
		{
			Ability_R();
		}
		else if (AnimName == "Aurora_Ability_F")
		{
			Ability_F();
		}
		else if (AnimName == "Aurora_Ability_F_Finish")
		{
			Ability_E();
		}
	}

}

void UMyAnimNotify_PlayerAbility::Ability_E()
{
	Player->Ability_E_Implementation();
}

void UMyAnimNotify_PlayerAbility::Ability_R()
{
	Player->GetCharacterMovement()->JumpZVelocity = 600.f;
	Player->Jump();
}

void UMyAnimNotify_PlayerAbility::Ability_F()
{
	if (Player->IsPlaying(Player->Ability_FMontage) == true)
	{
		Player->Ability_F_AddDamage();
	}
	else
	{
		Player->Ability_F_Finish();
	}
}
