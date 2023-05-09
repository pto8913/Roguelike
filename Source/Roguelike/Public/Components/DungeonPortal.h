// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonPortal.generated.h"

class URoguelikeGameInstance;
class UCapsuleComponent;
class UParticleSystemComponent;
class UParticleSystem;
class USceneComponent;

UCLASS()
class ROGUELIKE_API ADungeonPortal : public AActor
{
	GENERATED_BODY()

		UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;
	URoguelikeGameInstance* GetGameInstance();

	///////////////////////////////////////
	//
	UFUNCTION()
		void OnBeginOverlap(
			class AActor* OverlapActor,
			class AActor* OtherActor
		);

	UPROPERTY(EditAnywhere, Category = "Tutorial")
		bool bAllowOverlap = true;
public:
	// Sets default values for this actor's properties
	ADungeonPortal();

	UPROPERTY(VisibleAnywhere)
		UCapsuleComponent* CapsuleCollision = nullptr;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* SceneComponent = nullptr;

	///////////////////////////////////////
	// Particle

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* ParticleSysComponent = nullptr;

	///////////////////////////////////////
	// Portal

	UPROPERTY(EditAnywhere)
		UParticleSystem* GateParticle = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
