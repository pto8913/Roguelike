// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Public/Templates/ptoEnums.h"
#include "ptoStructs.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API UptoStructs : public UObject
{
	GENERATED_BODY()
	
};

USTRUCT()
struct FScores
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
		uint32 DefeatEnemy;
	UPROPERTY()
		uint32 Floor;
	UPROPERTY()
		float PlayTime;

	FScores(const uint32& Defeat, const uint32& InFloor, const float& InPlayTime)
		: DefeatEnemy(Defeat), Floor(InFloor), PlayTime(InPlayTime) {}

	FScores() : FScores(0, 0, 0.f) {}
};

USTRUCT()
struct FOptions
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
		float CameraArmLength;
	UPROPERTY()
		uint16 GraphicIndex;
	UPROPERTY()
		EGraphicType GraphicType;

	FOptions(const float CameraArmLength)
		: CameraArmLength(CameraArmLength) {}

	FOptions() : FOptions(300.f) {};
};

USTRUCT()
struct FDungeonInfos
{
	GENERATED_USTRUCT_BODY()
public:
	//////////////////
	// Roguelike
	UPROPERTY()
		TArray<ID> Dungeon;
	UPROPERTY()
		uint16 DungeonWidth;
	UPROPERTY()
		uint16 DungeonHeight;
	UPROPERTY()
		FVector DungeonPlayerStartLocation;
	/* StairLocation */
	UPROPERTY()
		FVector DungeonPlayerEndLocation;

	//////////////////
	// Maze
	UPROPERTY()
		TArray<ID> Maze;
	UPROPERTY()
		float MazeWidth;
	UPROPERTY()
		float MazeHeight;
	UPROPERTY()
		FVector MazePlayerStartLocation;
	/* PortalLocation (PortalType: MazeToMain) */
	UPROPERTY()
		FVector MazePlayerEndLocation;

	//////////////////
	// Exterior
	UPROPERTY()
		bool bIsSpawnPortal;
	UPROPERTY()
		FVector PortalLocation;
	UPROPERTY()
		EPortalType PortalType;
	UPROPERTY()
		uint16 SizeMaster;
	UPROPERTY()
		EPortalType StairType;
	
	//////////////////
	// For player
	UPROPERTY()
		float PlayerFullHealth;

	UPROPERTY()
		float PlayerHealth;

	//////////////////
	//
	UPROPERTY()
		EActiveMode ActiveMode;
	UPROPERTY()
		float PlayTime;
	UPROPERTY()
		uint32 DefeatEnemyCount;
	UPROPERTY()
		uint32 FloorCount = 1;

	///////////////////
	// Enemy
	UPROPERTY()
		TArray<UClass*> EnemyClasses;
	UPROPERTY()
		TArray<FName> EnemyNames;
	UPROPERTY()
		TArray<FTransform> EnemyTransforms;
	UPROPERTY()
		TArray<float> EnemyFullHealth;
	UPROPERTY()
		TArray<float> EnemyHealth;

	void InitEnemyInfo(uint32 Size)
	{
		EnemyClasses.Init(UClass::StaticClass(), Size);
		EnemyNames.Init(FName(), Size);
		EnemyTransforms.Init(FTransform(), Size);
		EnemyFullHealth.Init(0.f, Size);
		EnemyHealth.Init(0.f, Size);
	};

	FDungeonInfos()
	{
		Dungeon = {};
		DungeonWidth = 0;
		DungeonHeight = 0;
		DungeonPlayerStartLocation = FVector::ZeroVector;
		DungeonPlayerEndLocation = FVector::ZeroVector;
		Maze = {};
		MazeWidth = 0.f;
		MazeHeight = 0.f;
		MazePlayerStartLocation = FVector::ZeroVector;
		MazePlayerEndLocation = FVector::ZeroVector;
		bIsSpawnPortal = false;
		PortalLocation = FVector::ZeroVector;
		PortalType = EPortalType::None;
		SizeMaster = 0;
		StairType = EPortalType::None;
		PlayerFullHealth = 0.f;
		PlayerHealth = 0.f;
		ActiveMode = EActiveMode::None;
		PlayTime = 0.f;
		DefeatEnemyCount = 0;
		FloorCount = 1;
		EnemyClasses = {};
		EnemyNames = {};
		EnemyTransforms = {};
		EnemyFullHealth = {};
		EnemyHealth = {};
	}
};

