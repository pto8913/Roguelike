// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/DungeonExterior.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine.h"
// Sets default values
ADungeonExterior::ADungeonExterior()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Comp"));
	SetRootComponent(SceneComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> FindMeshObj(TEXT("/Engine/BasicShapes/Cube"));
	MeshObj = FindMeshObj.Object;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(SceneComponent);
	MeshComp->SetStaticMesh(MeshObj);
	MeshComp->SetCollisionProfileName("BlockAllDynamic");

	//////////////////////////////////
	// Wall	
	ConstructorHelpers::FObjectFinder<UMaterialInterface> WallMat(TEXT("/Game/Pack/Material/M_StoneWall_Inst.M_StoneWall_Inst"));
	WallMaterial = WallMat.Object;

	//////////////////////////////////
	// Floor
	ConstructorHelpers::FObjectFinder<UMaterialInterface> FloorMat(TEXT("/Game/Pack/Material/M_BrickFloor_Clean_Inst.M_BrickFloor_Clean_Inst"));
	FloorMaterial = FloorMat.Object;
}

void ADungeonExterior::SetWallMaterial()
{
	MeshComp->SetMaterial(0, WallMaterial);
	Tags.Add("Wall");
}


void ADungeonExterior::SetMeshRelativeScale(float x, float y, float z)
{
	MeshComp->SetRelativeScale3D(FVector(x, y, z));
}

void ADungeonExterior::SetMeshRelativeScale(FVector InScale)
{
	MeshComp->SetRelativeScale3D(InScale);
}


void ADungeonExterior::SetMeshRelativeLocation(float x, float y, float z)
{
	MeshComp->SetRelativeLocation(FVector(x, y, z));
}


void ADungeonExterior::SetMeshRelativeRotation(float Pitch, float Yaw, float Roll)
{
	MeshComp->SetRelativeRotation(FRotator(Pitch, Yaw, Roll));
}


void ADungeonExterior::SetFloorMaterial()
{
	MeshComp->SetMaterial(0, FloorMaterial);
	Tags.Add("Floor");
}


void ADungeonExterior::CompAttachToMesh(AActor* InComp)
{
	InComp->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepRelativeTransform);
}
