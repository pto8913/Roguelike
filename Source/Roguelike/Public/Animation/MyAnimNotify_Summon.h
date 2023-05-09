// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MyAnimNotify_Summon.generated.h"

class AKing;
class AMage;
class USkeletalMeshComponent;
class UAnimSequenceBase;

UCLASS()
class ROGUELIKE_API UMyAnimNotify_Summon : public UAnimNotify
{
	GENERATED_BODY()

		UFUNCTION()
		virtual void Notify(
			USkeletalMeshComponent* InMeshComp,
			UAnimSequenceBase* InAnimation
		);

	UPROPERTY()
		FString AnimName = "";

	UPROPERTY()
		USkeletalMeshComponent* MeshComp = nullptr;

	UPROPERTY()
		AKing* King = nullptr;

	UPROPERTY()
		AMage* Mage = nullptr;
};
