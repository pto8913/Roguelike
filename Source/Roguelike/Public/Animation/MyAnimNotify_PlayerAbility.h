// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimSequenceBase.h"
#include "Engine/World.h"
#include "MyAnimNotify_PlayerAbility.generated.h"

class ARoguelikeCharacter;
class USkeletalMeshComponent;
class UAnimInstance;

/**
 *
 */
UCLASS()
class ROGUELIKE_API UMyAnimNotify_PlayerAbility : public UAnimNotify
{
	GENERATED_BODY()

		UFUNCTION()
		void Notify(
			USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation
		) override;

	UPROPERTY()
		USkeletalMeshComponent* MeshComp = nullptr;

	UPROPERTY()
		ARoguelikeCharacter* Player = nullptr;

	UPROPERTY()
		UWorld* world = nullptr;

	///////////////////////////////
	// Animation
	UPROPERTY()
		FString AnimName = "";

	UPROPERTY()
		UAnimInstance* AnimInstance = nullptr;

	///////////////////////////////
	// Attack

	UPROPERTY()
		FVector CurrentLocation = FVector::ZeroVector;

	UPROPERTY()
		FRotator CurrentRotation = FRotator::ZeroRotator;

	void Ability_E();
	void Ability_R();
	void Ability_F();
};
