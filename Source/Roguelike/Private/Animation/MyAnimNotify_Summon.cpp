// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Animation/MyAnimNotify_Summon.h"

#include "Public/Enemy/King.h"
#include "Public/Enemy/Mage.h"

#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"

void UMyAnimNotify_Summon::Notify(
	USkeletalMeshComponent* InMeshComp,
	UAnimSequenceBase* InAnimation
)
{
	AnimName = InAnimation->GetName();
	MeshComp = InMeshComp;

	Mage = Cast<AMage>(MeshComp->GetOwner());
	if (Mage != nullptr)
	{
		if (AnimName == "Mage_SummonMontage")
		{
			Mage->mSummon();
			return;
		}
	}

	King = Cast<AKing>(MeshComp->GetOwner());
	if (King != nullptr)
	{
		if (AnimName == "King_RaiseDeadMontage")
		{
			King->mSummon();
		}
	}
}

