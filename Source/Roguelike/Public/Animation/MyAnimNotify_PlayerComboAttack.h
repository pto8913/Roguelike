// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimSequenceBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "MyAnimNotify_PlayerComboAttack.generated.h"

class ARoguelikeCharacter;

/**
 *
 */
UCLASS()
class ROGUELIKE_API UMyAnimNotify_PlayerComboAttack : public UAnimNotify
{
	GENERATED_BODY()

		virtual void Notify(
			USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation
		) override;

	UPROPERTY()
		ARoguelikeCharacter* Player = nullptr;

	UPROPERTY()
		USkeletalMeshComponent* MeshComp = nullptr;

	UPROPERTY()
		UWorld* world = nullptr;

};
