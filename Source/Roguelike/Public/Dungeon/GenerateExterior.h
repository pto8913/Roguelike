// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/Components/DungeonExterior.h"
#include "Public/Components/DungeonPortal.h"
#include "Public/Components/Stair.h"
#include "Public/Dungeon/GenerateDungeon.h"
#include "Public/Dungeon/GenerateMaze.h"
#include "Public/Templates/MyMath.h"
#include "Public/Templates/ptoGenerator.h"
#include "Public/Templates/ptoEnums.h"
#include "Public/Templates/ptoStructs.h"
#include "Public/RoguelikeGameInstance.h"
#include "GenerateExterior.generated.h"

class ARoguelikeGameMode;
class AMazeGameMode;
class ARoguelikeCharacter;
class UNotifierWidget;

UCLASS()
class ROGUELIKE_API AGenerateExterior : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UWorld* world = nullptr;
	UWorld* CheckWorld();

	UPROPERTY()
		ARoguelikeGameMode* RoguelikeGameMode = nullptr;

	UPROPERTY()
		AMazeGameMode* MazeGameMode = nullptr;

	UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;
	URoguelikeGameInstance* GetGameInstance();

	UPROPERTY()
		AGenerateDungeon* GenerateDungeon = nullptr;

	UPROPERTY()
		AGenerateMaze* GenerateMaze = nullptr;

	UPROPERTY()
		float SizeMaster = 500;

	UPROPERTY()
		float AdjLoc = SizeMaster / 2;

	UPROPERTY()
		float SizeMastering = SizeMaster / 100;

	UPROPERTY()
		float SpawnHeight = 0.f;

	UPROPERTY()
		uint16 MinWidth = 10;

	UPROPERTY()
		uint16 MinHeight = 10;

	UPROPERTY()
		uint16 MaxWidth = 60;

	UPROPERTY()
		uint16 MaxHeight = 60;

	UPROPERTY()
		float FloorWidth = 0.f;

	UPROPERTY()
		float FloorHeight = 0.f;

	UPROPERTY()
		FTransform Trans = FTransform::Identity;

	UPROPERTY()
		TArray<ID> Result = {};

	MyMath Math = MyMath();
	UPROPERTY()
		UptoGenerator* Generator = nullptr;

	UPROPERTY()
		TArray<int8> dx = { 0, 0, -1, 1 };

	UPROPERTY()
		TArray<int8> dy = { 1, -1, 0, 0 };

	///////////////////////////////////////
	// Make Wall

	/* Prevent players from going out of dungeon when using Ability_E. */
	void CheckCrack(const uint16 PosX, const uint16 PosY);

	/* Check all directions ID::Path or ID::Room, ID::EndOfPath. */
	void CheckCrack(TArray<Direction>& Dirs, const uint16 PosX, const uint16 PosY);

	/*
	@ Dirs, result of FrontOfWalkableDir.
	@ Dirs size == 1, normal wall.
	@ Dirs size >= 2, generate cube wall. because
		When a player teleports (use Ability_E), the player may pass through a wall.
	*/
	void GenerateWall(
		const FName Tag = "",
		const TArray<Direction> Dirs = {},
		const bool bIsSetTorch = false
	);

	/*
		Make Wall
	@ Tag, Dungeon or Maze. for delete, destroy, hidden
	@ bIsSetTorch, if true make torch.
	@ WalkableDir, check ID is Path or Room direction.
	*/
	void GenerateWall(
		FName Tag = "",
		bool bIsSetTorch = false,
		Direction WalkableDir = Direction::None
	);

	/*
		Make Wall Generate cube wall
	*/
	void GenerateWallWithTag(
		FName Tag = "",
		bool bIsSetTorch = false,
		Direction WalkableDir = Direction::None
	);

	UPROPERTY()
		UClass* TorchComp = nullptr;
	UPROPERTY()
		AActor* Torch = nullptr;
	UPROPERTY()
		ADungeonExterior* ExtWall = nullptr;

	///////////////////////////////////////
	// Make Floor

	// Make floor and roof with tag.
	void GenerateFloor(
		FName Tag = "",
		bool bIsMakeRoof = false,
		FVector InScale = FVector::ZeroVector,
		FVector InLocation = FVector::ZeroVector
	);
	void GenerateFloor(
		FName Tag = "",
		bool bIsMakeRoof = false,
		uint16 PosX = 0,
		uint16 PosY = 0
	);

	UPROPERTY()
		ADungeonExterior* ExtFloor = nullptr;
	UPROPERTY()
		ADungeonExterior* ExtRoof = nullptr;


	///////////////////////////////////////
	// Make Portal

	/*
		Make Portal.
	@ InType, type.
	@ bIsDungeon, if treu, make next floor stair.
	*/
	void SpawnPortal(EPortalType InType);
	UPROPERTY()
		UNotifierWidget* NotifierWidget = nullptr;

	void SpawnStair(EPortalType InType);

	///////////////////////////////////////
	// Template

	// Get dungeon or maze size.
	void GetActualSize();
	UPROPERTY()
		uint16 ActualWidth = 0;
	UPROPERTY()
		uint16 ActualHeight = 0;

	// For get generate exterior location x.
	UPROPERTY()
		float HorizonLocation = 0.f;

	// For get generate exterior location y.
	UPROPERTY()
		float VerticalLocation = 0.f;

	///////////////////////////////////////
	// Player Start / End Location

	UPROPERTY()
		TArray<FVector> PlayerStartCandidate;
	UPROPERTY()
		uint16 StartCandidateSize = 0;
	/*
		Get Player start location.
	@ bIsDungeon, if true get `Dungeon` start location. else `Maze`
	*/
	void GetStartCandidate(bool bIsDungeon);
	// Get Player end location.
	void GetEndCandidate(bool bIsDungeon);
	int32 GetRndStartCandidateIndex();

	void SetFloorSize();
	void Initialize();

	EOpenLevelType OpenType = EOpenLevelType::None;

	UPROPERTY()
		FOptions Options;
public:
	AGenerateExterior();

	void GenerateExterior();

	void SetSpawnHeight(float InValue);

	/* for calculate horizon/vertical location */
	uint16 GetSizeMaster() { return SizeMaster; }
	void SetSizeMaster(float InValue)
	{
		SizeMaster = InValue;
		SizeMastering = InValue / 100;
		AdjLoc = SizeMaster / 2;
		SetFloorSize();
	}

	void GenerateMazeExterior();
	AGenerateMaze* GetGenerateMaze() { return GenerateMaze; }

	/*  */
	void GenerateDungeonExterior();
	AGenerateDungeon* GetGenerateDungeon() { return GenerateDungeon; }

};
