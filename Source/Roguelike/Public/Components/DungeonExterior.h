// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonExterior.generated.h"

class USceneComponent;
class UStaticMesh;
class UStaticMeshComponent;
class UMaterialInterface;

UCLASS()
class ROGUELIKE_API ADungeonExterior : public AActor
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere)
		USceneComponent* SceneComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
		UStaticMesh* MeshObj = nullptr;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* MeshComp = nullptr;

	//////////////////////////////////////////
	// Wall

	UPROPERTY(VisibleAnywhere)
		UMaterialInterface* WallMaterial = nullptr;

	//////////////////////////////////////////
	// Floor

	UPROPERTY(VisibleAnywhere)
		UMaterialInterface* FloorMaterial = nullptr;

public:
	// Sets default values for this actor's properties
	ADungeonExterior();

	void SetWallMaterial();
	void SetMeshRelativeScale(float x, float y, float z);
	void SetMeshRelativeScale(FVector InScale);
	void SetMeshRelativeLocation(float x, float y, float z);
	void SetMeshRelativeRotation(float Pitch, float Yaw, float Roll);

	void SetFloorMaterial();

	void CompAttachToMesh(AActor* InComp);
};
