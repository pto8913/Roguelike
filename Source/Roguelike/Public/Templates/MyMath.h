// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ROGUELIKE_API MyMath
{
	// Dungeon / Maze actual size.
	void SetActualSize(const uint16 Width, const uint16 Heihgt);
	uint16 ActualWidth = 50;
	uint16 ActualHeight = 50;

	// for calculate horizon/vertical location
	void SetMasterSize(const uint16 Size);
	uint16 SizeMaster = 500;
	uint16 SizeMastering = 5;

	/*
		Calculate Game World Location from array Index.
		For get radius and horizon, vertical location.
	*/
	void SetFloorSize();
	float FloorWidth = 0;
	float FloorHeight = 0;
public:
	MyMath() { SetFloorSize(); };
	MyMath(const uint16 Width, const uint16 Heihgt) {
		SetActualSize(Width, Heihgt);
		SetFloorSize();
	};
	MyMath(const uint16 Size, const uint16 Width, const uint16 Heihgt)
	{
		SetMasterSize(Size);
		SetActualSize(Width, Heihgt);
		SetFloorSize();
	}

	// Get array index
	inline uint32 GetPos(const uint16 PosX, const uint16 PosY);

	inline bool IsNotOuterWall(const uint32 Pos);

	/*
		Get the actual size of grid dungeon / maze.
		Radius is length of Array center to outer wall
	@ Size, floor height / width.
	*/
	inline float GridRadius(const float Size);
	// Convert grid index to world location X.
	inline float GetWorldHorizontalLocation(const uint16 PosX);
	// Convert grid index to world location Y.
	inline float GetWorldVerticalLocation(const uint16 PosY);

	/*
		Get grid index from location pos.
		a little error. (because float)
	@ Pos, world location.
	@ bIsX, if true Location X.
	*/
	inline float GetGridPosFromLocationPos(const float Pos, const bool bIsX);

	/*
		Get the actual size of the world dungeon / maze.
	@ Size, actual height / width.
	@ Pos, world position.
	*/
	inline uint16 WorldRadius(const uint16 Size, const float Pos);

	inline bool IsOdd(const uint16 InValue);

	inline bool IsInWidth(const uint16 PosX);
	inline bool IsInHeight(const uint16 PosY);
};
