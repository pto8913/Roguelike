// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ptoEnums.generated.h"

UCLASS()
class ROGUELIKE_API UptoEnums : public UObject
{
	GENERATED_BODY()
	
};

UENUM()
enum class Direction : uint8
{
	Vertical = 0,
	Up = 1,
	Down = 2,
	Right = 3,
	Left = 4,
	None = 254,
	Horizontal = 255,
};
ENUM_CLASS_FLAGS(Direction)

UENUM()
enum class ID : uint8
{
	None = 255,
	Room = 1,
	RoomAble = 2,
	Wall = 3,
	OuterWall = 4,
	Path = 5,
	Border = 6,
	Water = 7,
	EndOfPath = 8
};
ENUM_CLASS_FLAGS(ID)

UENUM()
enum class EOpenLevelType : uint8
{
	Title = 0,
	Main = 1,
	Load = 2,
	Tutorial = 3,
	Maze = 4,
	Score = 5,
	None = 255,
};
ENUM_CLASS_FLAGS(EOpenLevelType)

UENUM()
enum class EActiveMode : uint8
{
	None = 255,
	Dungeon = 0,
	Maze = 1,
};

UENUM()
enum class EPortalType : uint8
{
	IntoMaze = 0,
	GetOutMaze = 1,
	IntoBossRoom = 2,
	NextFloor = 3,
	None = 255,
};
ENUM_CLASS_FLAGS(EPortalType)

UENUM()
enum class EGraphicType : uint8
{
	None = 255,
	High = 0,
	Mid = 1,
	Low = 2,
};
ENUM_CLASS_FLAGS(EGraphicType)