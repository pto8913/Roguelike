// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Dungeon/GenerateDungeon.h"

AGenerateDungeon::AGenerateDungeon()
{
	Generator = CreateDefaultSubobject<UptoGenerator>(TEXT("Generator"));
}

void AGenerateDungeon::GenerateDungeon()
{
	DungeonInitialize();

	MakeRect();

	MakeRoom();

	MergePath();

	SetPathWithWall();

	FindEndPath();

	SetOuterWall();

	FindCloseRoom();

	PathEnds.Empty();

	FindEndPath();

	SetPathWithWall();

	IsSpawnMazePortal();

	//DebugDungeon();
}

void AGenerateDungeon::DungeonInitialize()
{
	Math = MyMath();

	WallXs = {};
	WallYs = {};
	RectPos = {};
	RoomAllInfo = {};
	PathPoses = {};
	PathEnds = {};

	Dungeon.Init(ID::RoomAble, ActualHeight * ActualWidth);
}


/////////////////////////////////////////////////////////////
// 

void AGenerateDungeon::MakeRect()
{
	Direction Dir = GetRndHorizonOrVertical();

	if (Dir == Direction::Horizontal)
	{
		// Get position Y for make horizontal wall.
		GetWallYs(0, ActualWidth);

		uint16 PreRectY = 0;
		uint16 NextY = 0;
		uint32 WallYsSize = WallYs.Num();
		for (uint32 IdxY = 0; IdxY <= WallYsSize; ++IdxY)
		{
			if (IdxY == WallYsSize)
			{
				NextY = ActualHeight;
			}
			else
			{
				NextY = WallYs[IdxY];
			}

			if (NextY == 0) continue;

			GetWallXs(PreRectY, NextY, true);

			PreRectY = NextY;
		}
	}
	else if (Dir == Direction::Vertical)
	{
		// Get position X for make vertical wall.
		GetWallXs(0, ActualHeight);

		uint16 PreRectX = 0;
		uint16 NextX = 0;
		uint32 WallXsSize = WallXs.Num();
		for (uint32 IdxX = 0; IdxX <= WallXsSize; ++IdxX)
		{
			if (IdxX == WallXsSize)
			{
				NextX = ActualWidth;
			}
			else
			{
				NextX = WallXs[IdxX];
			}

			if (NextX == 0) continue;

			GetWallYs(PreRectX, NextX, true);

			PreRectX = NextX;
		}
	}
}

void AGenerateDungeon::GetWallYs(
	uint32 PreRectX,
	uint32 NextX,
	bool bIsSecond
)
{
	uint16 NumOfWall = FMath::RandRange(1, MaxNumOfWallY);

	for (int Idx = 0; Idx < NumOfWall; ++Idx)
	{
		// Need Position Y for make horizontal wall.
		WallYs.Add(GetWallRnd(ActualHeight, MaxNumOfWallY));
	}

	WallYs.Sort();

	uint16 PreY = 0;
	uint16 NextY = 0;
	uint32 WallYsSize = WallYs.Num();
	for (uint32 IdxY = 0; IdxY < WallYsSize; ++IdxY)
	{
		NextY = WallYs[IdxY];

		if (NextY - PreY < MINIMUM_RECT)
		{
			// Wall size must be at least larger than MINIMUM_RECT
			uint32 CheckIndex = WallYs.IndexOfByKey(NextY);
			if (WallYs.IsValidIndex(CheckIndex))
			{
				WallYs[CheckIndex] = 0;
			}
		}
		else
		{
			if (bIsSecond == true)
			{
				// Add rect's Lower Left Corner and Upper Right Corner position.
				RectPos.Add(
					RECTS(PreRectX, PreY, NextX, NextY)
				);
			}

			Generator->SetID(ID::Border, Dungeon, PreRectX, NextX, NextY, true);

			PreY = NextY;
		}
	}

	if (bIsSecond == true)
	{
		// Add rect's Lower Left Corner and Upper Right Corner position.
		RectPos.Add(
			RECTS(PreRectX, PreY, NextX, ActualHeight)
		);

		WallYs.Empty();
	}
}

void AGenerateDungeon::GetWallXs(
	uint32 PreRectY,
	uint32 NextY,
	bool bIsSecond
)
{
	uint16 NumOfWall = FMath::RandRange(1, MaxNumOfWallX);

	for (uint32 Idx = 0; Idx < NumOfWall; ++Idx)
	{
		// Need Position X for make vertical wall.
		WallXs.Add(GetWallRnd(ActualWidth, MaxNumOfWallX));
	}

	WallXs.Sort();

	uint16 PreX = 0;
	uint16 NextX = 0;
	uint32 WallXsSize = WallXs.Num();
	for (uint32 IdxX = 0; IdxX < WallXsSize; ++IdxX)
	{
		NextX = WallXs[IdxX];

		if (NextX - PreX < MINIMUM_RECT)
		{
			// Wall size must be at least larger than MINIMUM_RECT
			uint32 CheckIndex = WallXs.IndexOfByKey(NextX);
			if (WallXs.IsValidIndex(CheckIndex) == true)
			{
				WallXs[CheckIndex] = 0;
			}
		}
		else
		{
			if (bIsSecond == true)
			{
				// Add rect's Lower Left Corner and Upper Right Corner position.
				RectPos.Add(
					RECTS(PreX, PreRectY, NextX, NextY)
				);
			}

			Generator->SetID(ID::Border, Dungeon, PreRectY, NextY, NextX, false);

			PreX = NextX;
		}
	}
	if (bIsSecond == true)
	{
		// Add rect's Lower Left Corner and Upper Right Corner position.
		RectPos.Add(
			RECTS(PreX, PreRectY, ActualWidth, NextY)
		);

		WallXs.Empty();
	}
}


//////////////////////////////////////////////////////////////
// Make Room

void AGenerateDungeon::MakeRoom()
{
	uint16 MinX = 0, MinY = 0, MaxX = 0, MaxY = 0;

	uint16 RoomWidth = 0, RoomHeight = 0;

	uint16 RoomMinX = 0, RoomMaxX = 0;
	uint16 RoomMinY = 0, RoomMaxY = 0;
	for (const RECTS& Rects : RectPos)
	{
		RoomInfos RoomInfo = RoomInfos();

		Generator->GetItem(Rects, MinX, MinY, MaxX, MaxY);

		RoomInfo.SetRects(MinX, MinY, MaxX, MaxY);

		bIsNotUpOuterWall = Math.IsNotOuterWall(MaxY);
		bIsNotDownOuterWall = Math.IsNotOuterWall(MinY);
		bIsNotLeftOuterWall = Math.IsNotOuterWall(MinX);
		bIsNotRightOuterWall = Math.IsNotOuterWall(MaxX);

		RoomInfo.SetAllDirection(bIsNotUpOuterWall, bIsNotDownOuterWall, bIsNotLeftOuterWall, bIsNotRightOuterWall);

		AdjustWallPos(MinX, true);
		AdjustWallPos(MaxX);
		AdjustWallPos(MinY, true);
		AdjustWallPos(MaxY);

		RoomWidth = MaxX - MinX;
		RoomHeight = MaxY - MinY;

		if (RoomWidth <= 0 || RoomHeight <= 0) continue;

		RoomMinX = FMath::RandRange(MinX, MinX + RoomWidth / 2 - 2);
		RoomMaxX = FMath::RandRange(MinX + RoomWidth / 2 + 2, MaxX);
		if (RoomMinX > RoomMaxX)
		{
			Swap(RoomMinX, RoomMaxX);
		}

		RoomMinY = FMath::RandRange(MinY, MinY + RoomHeight / 2 - 2);
		RoomMaxY = FMath::RandRange(MinY + RoomHeight / 2 + 2, MaxY);
		if (RoomMinY > RoomMaxY)
		{
			Swap(RoomMinY, RoomMaxY);
		}

		RoomWidth = RoomMaxX - RoomMinX;
		RoomHeight = RoomMaxY - RoomMinY;

		// the room Top wall and Bottom wall.
		Generator->SetID(ID::Wall, Dungeon, RoomMinX, RoomMaxX, RoomMinY, true, true, RoomMaxY);

		// the room Left wall and Right wall.
		Generator->SetID(ID::Wall, Dungeon, RoomMinY, RoomMaxY, RoomMinX, false, true, (uint16)(RoomMaxX - 1));

		for (uint32 PosY = RoomMinY + 1; PosY < RoomMaxY; ++PosY)
		{
			// Set room's id
			Generator->SetID(ID::Room, Dungeon, RoomMinX + 1, RoomMaxX - 1, PosY, true);
		}

		RoomInfo.SetRooms(RoomMinX, RoomMinY, RoomMaxX, RoomMaxY);

		MakePath(RoomInfo);
	}

}


//////////////////////////////////////////////////////////////
// MakePath

void AGenerateDungeon::MakePath(RoomInfos& RoomInfo)
{
	uint16 RoomMinX = 0, RoomMinY = 0, RoomMaxX = 0, RoomMaxY = 0;
	RoomInfo.GetRooms(RoomMinX, RoomMinY, RoomMaxX, RoomMaxY);

	uint16 RectMinX = 0, RectMinY = 0, RectMaxX = 0, RectMaxY = 0;
	RoomInfo.GetRects(RectMinX, RectMinY, RectMaxX, RectMaxY);

	uint16 PathY = 0;
	uint16 PathX = 0;

	RoomInfo.GetAllDirection(
		bIsNotUpOuterWall,
		bIsNotDownOuterWall,
		bIsNotLeftOuterWall,
		bIsNotRightOuterWall
	);

	// Make a path from Top of the room
	if (bIsNotUpOuterWall == true)
	{
		PathX = FMath::RandRange(RoomMinX + 1, RoomMaxX - 2);
		Generator->SetID(ID::Path, Dungeon, PathPoses, RoomMaxY, RectMaxY + 1, PathX, false);

		RoomInfo.SetStartPathPos(PathX, RoomMaxY);
		RoomInfo.SetEndPathPos(PathX, RectMaxY);
	}

	// Make a path from Bottom of the room
	if (bIsNotDownOuterWall == true)
	{
		PathX = FMath::RandRange(RoomMinX + 1, RoomMaxX - 2);
		Generator->SetID(ID::Path, Dungeon, PathPoses, RectMinY, RoomMinY + 1, PathX, false);

		RoomInfo.SetStartPathPos(PathX, RoomMinY);
		RoomInfo.SetEndPathPos(PathX, RectMinY);
	}

	// Make a path from Left of the room
	if (bIsNotLeftOuterWall == true)
	{
		PathY = FMath::RandRange(RoomMinY + 1, RoomMaxY - 1);
		Generator->SetID(ID::Path, Dungeon, PathPoses, RectMinX, RoomMinX + 1, PathY, true);

		RoomInfo.SetStartPathPos(RoomMinX, PathY);
		RoomInfo.SetEndPathPos(RectMinX, PathY);
	}

	// Make a path from Right of the room
	if (bIsNotRightOuterWall == true)
	{
		PathY = FMath::RandRange(RoomMinY + 1, RoomMaxY - 1);
		Generator->SetID(ID::Path, Dungeon, PathPoses, RoomMaxX - 1, RectMaxX + 1, PathY, true);

		RoomInfo.SetStartPathPos(RoomMaxX - 1, PathY);
		RoomInfo.SetEndPathPos(RectMaxX, PathY);
	}

	RoomAllInfo.Add(RoomInfo);
}

void AGenerateDungeon::MergePath()
{
	uint16 MinX = 0, MinY = 0, MaxX = 0, MaxY = 0;
	for (RoomInfos& RoomInfo : RoomAllInfo)
	{
		RoomInfo.GetRects(MinX, MinY, MaxX, MaxY);

		RoomInfo.GetAllDirection(bIsNotUpOuterWall, bIsNotDownOuterWall, bIsNotLeftOuterWall, bIsNotRightOuterWall);

		if (bIsNotUpOuterWall == true)
		{
			SetPathOnBorder(MinX, MaxX, MaxY, true);
		}

		if (bIsNotDownOuterWall == true)
		{
			SetPathOnBorder(MinX, MaxX, MinY, true);
		}

		if (bIsNotLeftOuterWall == true)
		{
			SetPathOnBorder(MinY, MaxY, MinX, false);
		}

		if (bIsNotRightOuterWall == true)
		{
			SetPathOnBorder(MinY, MaxY, MaxX, false);
		}
	}
}

void AGenerateDungeon::SetPathOnBorder(
	uint16 MinPos, uint16 MaxPos,
	uint16 ConstantPos, bool bIsConstantHorizon
)
{
	uint32 CheckIndex = 0;
	TArray<uint32> Result = {};
	for (uint32 Pos = MinPos; Pos <= MaxPos; ++Pos)
	{
		if (bIsConstantHorizon == true)
		{
			CheckIndex = Math.GetPos(Pos, ConstantPos);
		}
		else
		{
			CheckIndex = Math.GetPos(ConstantPos, Pos);
		}

		if (Generator->IDIs(Dungeon, CheckIndex, ID::Path) == true)
		{
			Result.Add(Pos);
		}
	}
	if (Result.Num() > 1)
	{
		Result.Sort();

		Generator->SetID(ID::Path, Dungeon, PathPoses, Result[0], Result[1] + 1, ConstantPos, bIsConstantHorizon);
	}
}

void AGenerateDungeon::FindEndPath()
{
	uint16 PosX = 0, PosY = 0;
	uint32 Pos = 0;
	int Count = 0;

	// check all direction, then if ID::Path is one, it's a ID::EndOFPath
	for (RoomInfos& RoomInfo : RoomAllInfo)
	{
		for (uint32 Index = 0; Index < RoomInfo.GetEndPathPosSize(); ++Index)
		{
			Count = 0;
			RoomInfo.GetEndPathPos(Index, PosX, PosY);

			if (CheckIsEndOfPath(Count, PosX, PosY) == true)
			{
				Pos = Math.GetPos(PosX, PosY);
				if (Dungeon.IsValidIndex(Pos) == true)
				{
					Dungeon[Pos] = ID::EndOfPath;
					PathEnds.Add(PathPos(PosX, PosY));
				}
			}
		}
	}
}


void AGenerateDungeon::FindCloseRoom()
{
	uint16 PosX = 0, PosY = 0;
	uint32 Pos = 0;

	int Count = 0, non = 0;
	for (RoomInfos& RoomInfo : RoomAllInfo)
	{
		Count = 0;
		for (uint32 Idx = 0; Idx < RoomInfo.GetEndPathPosSize(); ++Idx)
		{
			non = 0;

			RoomInfo.GetEndPathPos(Idx, PosX, PosY);

			Pos = Math.GetPos(PosX, PosY);

			if (Generator->IDIs(Dungeon, Pos, ID::EndOfPath) == true)
			{
				Count += CheckIsEndOfPath(non, PosX, PosY);
			}
		}
		if (Count == RoomInfo.GetEndPathPosSize())
		{
			CloseRoomSetID(PosX, PosY);
		}
	}
}


bool AGenerateDungeon::CheckIsEndOfPath(int& Count, uint16 PosX, uint16 PosY)
{
	uint32 Pos = Math.GetPos(PosX + 1, PosY);
	if (Generator->IDIs(Dungeon, Pos, ID::Path) == true) ++Count;

	Pos = Math.GetPos(PosX - 1, PosY);
	if (Generator->IDIs(Dungeon, Pos, ID::Path) == true) ++Count;

	Pos = Math.GetPos(PosX, PosY + 1);
	if (Generator->IDIs(Dungeon, Pos, ID::Path) == true) ++Count;

	Pos = Math.GetPos(PosX, PosY - 1);
	if (Generator->IDIs(Dungeon, Pos, ID::Path) == true) ++Count;

	if (Count == 1)
	{
		return true;
	}
	return false;
}


void AGenerateDungeon::IsSpawnMazePortal()
{
	if (PathEnds.Num() == 0) return;

	if (FMath::RandRange(0, 5) == FMath::RandRange(0, 5))
	{
		UE_LOG(LogTemp, Log, TEXT("Spawn Portal"));
		// Spawn
		bSpawnPortal = true;
	}
}


bool AGenerateDungeon::IsSpawnPortal()
{
	return bSpawnPortal;
}


//////////////////////////////////////////////////////////////
// Generate sub

void AGenerateDungeon::SetOuterWall()
{
	Generator->SetID(ID::OuterWall, Dungeon, 0, ActualHeight, 0, false, true, ActualWidth - 1);
	Generator->SetID(ID::OuterWall, Dungeon, 0, ActualWidth, 0, true, true, ActualHeight - 1);
}


void AGenerateDungeon::CloseRoomSetID(uint16 PosX, uint16 PosY)
{
	const uint16 TX = PosX, TY = PosY;
	Direction Dir = Direction::Up;
	uint32 Pos = Math.GetPos(PosX, PosY);
	while (!Generator->IDIs(Dungeon, Pos, ID::Path))
	{
		if (Dir == Direction::Up) ++PosY;
		if (Dir == Direction::Down) --PosY;
		if (Dir == Direction::Left) --PosX;
		if (Dir == Direction::Right) ++PosX;

		Pos = Math.GetPos(PosX, PosY);
		if (Generator->IDIs(Dungeon, Pos, ID::OuterWall) == true)
		{
			if (Dir == Direction::Up) Dir = Direction::Down;
			else if (Dir == Direction::Down) Dir = Direction::Left;
			else if (Dir == Direction::Left) Dir = Direction::Right;
			PosX = TX;
			PosY = TY;
		}
	}

	if (Dir == Direction::Up)
	{
		Generator->SetID(ID::Path, Dungeon, PathPoses, TY, PosY + 1, TX, false);
	}
	if (Dir == Direction::Down)
	{
		Generator->SetID(ID::Path, Dungeon, PathPoses, PosY - 1, TY + 1, TX, false);
	}
	if (Dir == Direction::Left)
	{
		Generator->SetID(ID::Path, Dungeon, PathPoses, PosX, TX + 1, TY, true);
	}
	if (Dir == Direction::Right)
	{
		Generator->SetID(ID::Path, Dungeon, PathPoses, TX, PosX + 1, TY, true);
	}
}


void AGenerateDungeon::SetPathWithWall()
{
	uint16 PosX = 0, PosY = 0;
	uint32 Pos = 0;
	for (const PathPos& Poses : PathPoses)
	{
		Generator->GetItem(Poses, PosX, PosY);

		Pos = Math.GetPos(PosX + 1, PosY);
		ID Id = Dungeon[Pos];
		if ((Id != ID::Path) && (Id != ID::Room) && (Id != ID::EndOfPath))
		{
			Generator->SetIDPoint(ID::Wall, Dungeon, Pos);
		}

		Pos = Math.GetPos(PosX - 1, PosY);
		Id = Dungeon[Pos];
		if ((Id != ID::Path) && (Id != ID::Room) && (Id != ID::EndOfPath))
		{
			Generator->SetIDPoint(ID::Wall, Dungeon, Pos);
		}

		Pos = Math.GetPos(PosX, PosY + 1);
		Id = Dungeon[Pos];
		if ((Id != ID::Path) && (Id != ID::Room) && (Id != ID::EndOfPath))
		{
			Generator->SetIDPoint(ID::Wall, Dungeon, Pos);
		}

		Pos = Math.GetPos(PosX, PosY - 1);
		Id = Dungeon[Pos];
		if ((Id != ID::Path) && (Id != ID::Room) && (Id != ID::EndOfPath))
		{
			Generator->SetIDPoint(ID::Wall, Dungeon, Pos);
		}
	}
}


template<typename T, typename U>
uint16 AGenerateDungeon::GetWallRnd(T OneSide, U MaxNumOfWall)
{
	return FMath::RandRange(OneSide / MaxNumOfWall, OneSide / MaxNumOfWall * (MaxNumOfWall - 1));
}


Direction AGenerateDungeon::GetRndHorizonOrVertical()
{
	int tmp = FMath::RandRange(1, 2);
	if (tmp % 2 == 0) return Direction::Vertical;
	else return Direction::Horizontal;
}


// 部屋の壁は境界線から 3 マス以上離れさせる、外壁から1マス以上離れさせる
void AGenerateDungeon::AdjustWallPos(uint16& Pos, bool bIsMin)
{
	if (Pos == 0)
	{
		Pos += 1;
	}
	else if (Pos == ActualHeight - 1 || Pos == ActualWidth - 1)
	{
		Pos -= 1;
	}
	else if (bIsMin == true)
	{
		Pos += 3;
	}
	else if (bIsMin == false)
	{
		Pos -= 3;
	}
}

/////////////////////////////////////////////////////////////////
// Templates

void AGenerateDungeon::DebugDungeon()
{
	uint32 CheckIndex = 0;
	FString tmp = "";
	for (int MatY = 0; MatY < ActualHeight; ++MatY)
	{
		tmp = "";
		for (int MatX = 0; MatX < ActualWidth; ++MatX)
		{
			CheckIndex = Math.GetPos(MatX, MatY);
			if (Dungeon.IsValidIndex(CheckIndex) == true)
			{
				ID id = Dungeon[CheckIndex];
				if (id == ID::OuterWall)
				{
					tmp += "#";
				}
				else if (id == ID::Wall)
				{
					tmp += "@";
				}
				else if (id == ID::RoomAble)
				{
					tmp += "_";
				}
				else if (id == ID::Path)
				{
					tmp += ".";
				}
				else if (id == ID::Border)
				{
					tmp += "_";
				}
				else if (id == ID::Room)
				{
					tmp += " ";
				}
				else if (id == ID::EndOfPath)
				{
					tmp += "E";
				}
			}
			//UE_LOG(LogTemp, Log, TEXT("x: %d, y: %d"), MatX, MatY);
		}
		UE_LOG(LogTemp, Log, TEXT("%s"), *tmp);
	}
}


void AGenerateDungeon::GetActualWidth(uint16& OutWidth)
{
	OutWidth = ActualWidth;
}


void AGenerateDungeon::GetActualHeight(uint16& OutHeight)
{
	OutHeight = ActualHeight;
}


PathPos AGenerateDungeon::GetRndEndPos()
{
	uint32 Size = PathEnds.Num();
	if (Size == 0) return PathPos();
	uint32 Index = FMath::RandRange(0, Size - 1);
	return PathEnds[Index];
}