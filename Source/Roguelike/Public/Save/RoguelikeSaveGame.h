// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Public/Templates/ptoEnums.h"
#include "Public/Templates/ptoStructs.h"
#include "RoguelikeSaveGame.generated.h"

UCLASS()
class ROGUELIKE_API URoguelikeSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/////////////////////////////////////////
	//// Exterior
	//UPROPERTY()
	//	bool bIsSpawnPortal;

	//UPROPERTY()
	//	FVector PortalLocation;

	//UPROPERTY()
	//	EPortalType PortalType;

	//UPROPERTY()
	//	uint16 SizeMaster;

	///////////////////////////////////////////
	//// Dungeon
	//UPROPERTY()
	//	TArray<ID> Dungeon;

	//UPROPERTY()
	//	uint16 DungeonWidth;

	//UPROPERTY()
	//	uint16 DungeonHeight;

	//UPROPERTY()
	//	FVector DungeonPlayerStartLocation;

	//UPROPERTY()
	//	FVector DungeonPlayerEndLocation;

	///////////////////////////////////////////
	//// Maze
	//UPROPERTY()
	//	TArray<ID> Maze;

	//UPROPERTY()
	//	float MazeWidth;

	//UPROPERTY()
	//	float MazeHeight;

	//UPROPERTY()
	//	FVector MazePlayerStartLocation;

	//UPROPERTY()
	//	FVector MazePlayerEndLocation;

	///////////////////////////////////////////
	//// For player
	//UPROPERTY()
	//	float PlayerFullHealth;

	//UPROPERTY()
	//	float PlayerHealth;

	UPROPERTY()
		FDungeonInfos DungeonInfos;

	/////////////////////////////////////////
	// etc
	//UPROPERTY()
	//	TArray<UClass*> ActorClasses;

	//UPROPERTY()
	//	TArray<FName> ActorNames;

	//UPROPERTY()
	//	TArray<FTransform> ActorTransforms;

	//UPROPERTY()
	//	TArray<float> ActorFullHealth;

	//UPROPERTY()
	//	TArray<float> ActorHealth;

	////////////////////////////////////////////
	////
	//UPROPERTY()
	//	EActiveMode ActiveMode;

	UPROPERTY()
		FDateTime TimeStamp;

	//UPROPERTY()
	//	float PlayTime;

	//UPROPERTY()
	//	uint32 DefeatEnemyCount;

	UPROPERTY()
		FString SaveSlotName;

	UPROPERTY()
		uint16 Index;

	//UPROPERTY()
	//	uint32 FloorCount;

	UPROPERTY()
		FOptions Options;

	/////////////////////////////////////////
	//
	UPROPERTY()
		TArray<FScores> Score;

	void InitArray(uint32 Size);
};
