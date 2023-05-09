// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/Templates/MyMath.h"
#include "Public/Templates/ptoGenerator.h"
#include "GenerateDungeon.generated.h"

#define MINIMUM_RECT 11

typedef struct RoomInfos
{
	RoomInfos()
	{
		this->Rects.Init(-1, 4);
		this->Room.Init(-1, 4);
		this->EndPathPoses = {};
		this->StartPathPoses = {};
		this->AllDirection.Init(false, 4);
	}

	void GetRects(uint16& MinX, uint16& MinY, uint16& MaxX, uint16& MaxY)
	{
		GetItem(Rects, MinX, MinY, MaxX, MaxY);
	}

	void SetRects(const uint16 MinX, const uint16 MinY, const uint16 MaxX, const uint16 MaxY)
	{
		SetItem(Rects, MinX, MinY, MaxX, MaxY);
	}


	void GetRooms(uint16& MinX, uint16& MinY, uint16& MaxX, uint16& MaxY)
	{
		GetItem(Room, MinX, MinY, MaxX, MaxY);
	}

	void SetRooms(const uint16 MinX, const uint16 MinY, const uint16 MaxX, const uint16 MaxY)
	{
		SetItem(Room, MinX, MinY, MaxX, MaxY);
	}


	void GetEndPathPos(const uint32 Pos, uint16& PosX, uint16& PosY)
	{
		GetItem(EndPathPoses, Pos, PosX, PosY);
	}

	void SetEndPathPos(const uint16 PosX, const uint16 PosY)
	{
		SetItem(EndPathPoses, PosX, PosY);
	}

	uint32 GetEndPathPosSize()
	{
		return EndPathPoses.Num();
	}


	void GetAllDirection(bool& Up, bool& Down, bool& Left, bool& Right)
	{
		GetItem(AllDirection, Up, Down, Left, Right);
	}

	void SetAllDirection(const bool Up, const bool Down, const bool Left, const bool Right)
	{
		SetItem(AllDirection, Up, Down, Left, Right);
	}


	void GetStartPathPos(const uint32 Pos, uint16& PosX, uint16& PosY)
	{
		GetItem(StartPathPoses, Pos, PosX, PosY);
	}

	void SetStartPathPos(const uint16 PosX, const uint16 PosY)
	{
		SetItem(StartPathPoses, PosX, PosY);
	}

	uint32 GetStartPathPosSize()
	{
		return StartPathPoses.Num();
	}

private:
	TArray<uint16> Rects;
	TArray<uint16> Room;
	TArray<PathPos> EndPathPoses;
	TArray<PathPos> StartPathPoses;

	// Up Down Left Right
	TArray<bool> AllDirection;

	template<typename T>
	void SetItem(TArray<T>& Target, const T MinX, const T MinY, const T MaxX, const T MaxY)
	{
		Target[0] = MinX;
		Target[1] = MinY;
		Target[2] = MaxX;
		Target[3] = MaxY;
	}

	template<typename T>
	void SetItem(TArray<PathPos>& Target, const T PosX, const T PosY)
	{
		Target.Add(PathPos(PosX, PosY));
	}

	template<typename T>
	void GetItem(const TArray<T> Target, T& MinX, T& MinY, T& MaxX, T& MaxY)
	{
		MinX = Target[0];
		MinY = Target[1];
		MaxX = Target[2];
		MaxY = Target[3];
	}

	template<typename T>
	void GetItem(const TArray<T> Target, T& PosX, T& PosY)
	{
		PosX = Target[0];
		PosY = Target[1];
	}

	void GetItem(TArray<PathPos> Out, const uint32 Pos, uint16& PosX, uint16& PosY)
	{
		if (Out.IsValidIndex(Pos) == true)
		{
			GetItem(Out[Pos], PosX, PosY);
		}
	}

	void GetItem(const PathPos Target, uint16& PosX, uint16& PosY)
	{
		PosX = Target.Get<0>();
		PosY = Target.Get<1>();
	}

} ROOMINFOS;

UCLASS()
class ROGUELIKE_API AGenerateDungeon : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY()
		uint16 ActualWidth = 50;

	UPROPERTY()
		uint16 ActualHeight = 50;

	UPROPERTY()
		uint16 MaxNumOfWallY = 5;

	UPROPERTY()
		uint16 MaxNumOfWallX = 5;

	UPROPERTY()
		TArray<ID> Dungeon;

	UPROPERTY()
		bool bIsNotUpOuterWall = true;

	UPROPERTY()
		bool bIsNotDownOuterWall = true;

	UPROPERTY()
		bool bIsNotLeftOuterWall = true;

	UPROPERTY()
		bool bIsNotRightOuterWall = true;

	MyMath Math = MyMath();
	UPROPERTY()
		UptoGenerator* Generator = nullptr;

	void DungeonInitialize();

	/*
		At first, decide Horizon / Vertical Wall location.
		second,   decide Vertical / Horizon Wall location.
		veryveryveryveryvery good
	*/
	void MakeRect();

	// Retention Wall Y position
	UPROPERTY()
		TArray<uint16> WallYs;

	// Set WallYs
	void GetWallYs(
		uint32 PreRectX,
		uint32 NextX,
		bool bIsSecond = false
	);

	// Retention Wall X position
	UPROPERTY()
		TArray<uint16> WallXs;

	// Set WallXs
	void GetWallXs(
		uint32 PreRectY,
		uint32 NextY,
		bool bIsSecond = false
	);

	/*
		Retention rect's
		Lower Left Corner (LLC) Position and
		Upper Right Corner (URC) Position
	*/
	TArray<RECTS> RectPos;

	// Make Rect. This function create Outer Wall and Border Wall.
	void MakeRoom();

	void FindCloseRoom();

	bool CheckIsEndOfPath(int& Count, uint16 PosX, uint16 PosY);
	void MakePath(RoomInfos& RoomInfo);

	// All info set is finished in MakePath
	TArray<RoomInfos> RoomAllInfo;

	void MergePath();
	void SetPathOnBorder(
		uint16 MinPos, uint16 MaxPos,
		uint16 ConstantPos, bool bIsConstantHorizon
	);

	TArray<PathPos> PathPoses;

	/* Set PathEnds and EndOfPath */
	void FindEndPath();
	TArray<PathPos> PathEnds;

	void IsSpawnMazePortal();
	UPROPERTY()
		bool bSpawnPortal = false;

	// Start Generate Dungeon Sub Func

	void SetPathWithWall();

	void SetOuterWall();

	template<typename T, typename U>
	uint16 GetWallRnd(T OneSide, U MaxNumOfWall);

	Direction GetRndHorizonOrVertical();

	void DebugDungeon();

	void AdjustWallPos(uint16& MinX, bool bIsMin = false);

	/* FInish Sub Func */

	void CloseRoomSetID(uint16 PosX, uint16 PosY);

	///////////////////////////
	//

public:
	AGenerateDungeon();

	void GenerateDungeon();

	void GetActualWidth(uint16& OutWidth);
	void GetActualHeight(uint16& OutHeight);

	bool IsSpawnPortal();

	PathPos GetRndEndPos();

	TArray<ID> GetDungeon() { return Dungeon; }
};