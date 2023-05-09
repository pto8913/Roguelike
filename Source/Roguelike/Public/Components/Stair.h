// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stair.generated.h"

class URoguelikeGameInstance;
class UStaticMesh;
class UStaticMeshComponent;
class USceneComponent;
class UParticleSystemComponent;
class UParticleSystem;

UCLASS()
class ROGUELIKE_API AStair : public AActor
{
	GENERATED_BODY()

		UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;
	URoguelikeGameInstance* GetGameInstance();

	UPROPERTY(EditAnywhere)
		USceneComponent* SceneComponent = nullptr;

	UPROPERTY()
		UStaticMesh* Iwa = nullptr;

	UPROPERTY()
		UMaterialInterface* Material = nullptr;

	UPROPERTY()
		UStaticMeshComponent* Stair1MeshComp = nullptr;

	UPROPERTY()
		UStaticMeshComponent* Stair2MeshComp = nullptr;

	UPROPERTY()
		UStaticMeshComponent* Stair3MeshComp = nullptr;

	UPROPERTY()
		UStaticMeshComponent* Stair4MeshComp = nullptr;

	UPROPERTY()
		UParticleSystemComponent* ParticleSysComponent = nullptr;

	UPROPERTY()
		UParticleSystem* Portal = nullptr;
	///////////////////////////////////////
	//

	UFUNCTION()
		void OnHit(
			UPrimitiveComponent* HitComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			FVector NormalImpulse,
			const FHitResult& Hit
		);

	UPROPERTY(EditAnywhere, Category = "Tutorial")
		bool bAllowHit = true;
public:
	// Sets default values for this actor's properties
	AStair();

};
