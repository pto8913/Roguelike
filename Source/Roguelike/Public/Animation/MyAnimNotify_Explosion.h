// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimSequenceBase.h"
#include "Public/Attack/Explosion.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Public/Enemy/Mage.h"
#include "Public/Player/RoguelikeCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MyAnimNotify_Explosion.generated.h"
/**
 *
 */
UCLASS()
class ROGUELIKE_API UMyAnimNotify_Explosion : public UAnimNotify
{
	GENERATED_BODY()

		UFUNCTION()
		void Notify(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY()
		AMage* Mage = nullptr;

	UPROPERTY()
		ARoguelikeCharacter* Player = nullptr;

	UPROPERTY()
		USkeletalMeshComponent* MeshComp = nullptr;

	UPROPERTY()
		UWorld* world = nullptr;

	///////////////////////////////
	// Animation
	UPROPERTY()
		FString AnimName = "";

	///////////////////////////////
	// Bullet
	UPROPERTY()
		FTransform Trans = FTransform::Identity;

	UPROPERTY()
		FVector ExplosionSpawnLocation = FVector::ZeroVector;

	UPROPERTY()
		FRotator ExplosionSpawnRotation = FRotator::ZeroRotator;

	UPROPERTY()
		FVector ExplosionScale = FVector(0.3f, 0.3f, 0.3f);

	UPROPERTY()
		AExplosion* Explode = nullptr;

	void SpawnExplosion();

	void Explosion(AMage* InOwner);

	void Explosion(ARoguelikeCharacter* InOwner);
};
