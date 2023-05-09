// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/Templates/MyMath.h"
#include "Public/Templates/ptoGenerator.h"
#include "Public/Templates/ptoEnums.h"
#include "GenerateMaze.generated.h"

/*
	Use dig algorithm.
*/


UCLASS()
class ROGUELIKE_API AGenerateMaze : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY()
		TArray<ID> Maze;

	UPROPERTY()
		uint16 StartX = 1;

	UPROPERTY()
		uint16 StartY = 1;

	UPROPERTY()
		/* constant */
		uint16 MinWidth = 5;

	UPROPERTY()
		uint16 MaxWidth = 25;

	UPROPERTY()
		/* Must be Odd size */
		uint16 NewMaxWidth = MaxWidth;

	UPROPERTY()
		/* constant */
		uint16 MinHeight = 5;

	UPROPERTY()
		uint16 MaxHeight = 25;

	UPROPERTY()
		/* Must be Odd size */
		uint16 NewMaxHeight = MaxHeight;

	UPROPERTY()
		uint16 ActualWidth = 0;

	UPROPERTY()
		uint16 ActualHeight = 0;

	UPROPERTY()
		TArray<Direction> Dirs = {};

	MyMath Math = MyMath();
	UPROPERTY()
		UptoGenerator* Generator = nullptr;

	TArray<PathPos> PathPoses = {};

	///////////////////////////////////////////
	// initialize
	void SetMazeSize();
	void MazeInitialize();

	///////////////////////////////////////////
	// Generator
	bool IsDigAbleRight(const uint16 PosX, const uint16 PosY);
	bool IsDigAbleLeft(const uint16 PosX, const uint16 PosY);
	bool IsDigAbleUpper(const uint16 PosX, const uint16 PosY);
	bool IsDigAbleDown(const uint16 PosX, const uint16 PosY);
	bool GetDigStartPos();
	void SetReachPos(const uint16 PosX, const uint16 PosY);
	void StartDig();
	bool IsWall(const uint32 Pos);

	///////////////////////////////////////////
	// 
	void SetOuterWallOfMaze(const ID NewId);

	void DebugMaze();

public:
	AGenerateMaze();

	void GenerateMaze();

	TArray<ID> GetMaze() { return Maze; }

	void GetActualWidth(uint16& OutWidth);
	void GetActualHeight(uint16& OutHeight);

	void SetMaxSize(const uint16 InWidth, const uint16 InHeight);
};
