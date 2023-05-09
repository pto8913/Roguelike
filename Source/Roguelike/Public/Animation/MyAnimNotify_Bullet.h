// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimSequenceBase.h"
#include "Public/Attack/Bullet.h"
#include "Engine/World.h"
#include "MyAnimNotify_Bullet.generated.h"

class AMage;
class ACrossbowman;
class USkeletalMeshComponent;

/**
 *
 */
UCLASS()
class ROGUELIKE_API UMyAnimNotify_Bullet : public UAnimNotify
{
	GENERATED_BODY()

		UFUNCTION()
		void Notify(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY()
		AMage* Mage = nullptr;

	UPROPERTY()
		ACrossbowman* Crossbowman = nullptr;

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
		FVector FireSpawnLocation;

	UPROPERTY()
		FRotator FireSpawnRotation;

	UPROPERTY()
		FVector FireScale;

	UPROPERTY()
		ABullet* Bullet = nullptr;

	void SpawnBullet();

	void Fire(AMage* InOwner);

	void Fire(ACrossbowman* InOwner);
};
