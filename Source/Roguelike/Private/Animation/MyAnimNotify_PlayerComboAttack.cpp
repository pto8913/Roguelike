// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Animation/MyAnimNotify_PlayerComboAttack.h"

#include "Public/Player/RoguelikeCharacter.h"


void UMyAnimNotify_PlayerComboAttack::Notify(
	USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation
)
{
	MeshComp = InMeshComp;
	Player = Cast<ARoguelikeCharacter>(MeshComp->GetOwner());
	if (Player != nullptr)
	{
		if (Player->GetComboAttackSave() == true)
		{
			Player->SetComboAttackSave(false);
		}
	}
}