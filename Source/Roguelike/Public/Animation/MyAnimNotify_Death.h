// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameFramework/Character.h"
#include "Animation/AnimSequenceBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "MyAnimNotify_Death.generated.h"

/**
 *
 */
UCLASS()
class ROGUELIKE_API UMyAnimNotify_Death : public UAnimNotify
{
	GENERATED_BODY()

		virtual void Notify(
			USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation
		) override;

	UPROPERTY()
		UWorld* world = nullptr;
};
