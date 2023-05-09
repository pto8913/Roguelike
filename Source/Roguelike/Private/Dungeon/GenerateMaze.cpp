// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Dungeon/GenerateMaze.h"

AGenerateMaze::AGenerateMaze()
{
	Generator = CreateDefaultSubobject<UptoGenerator>(TEXT("Generator"));
}

///////////////////////////////////////////
// initialize

void AGenerateMaze::SetMazeSize()
{
	NewMaxWidth = FMath::RandRange(10, MaxWidth);
	NewMaxHeight = FMath::RandRange(10, MaxHeight);
	ActualWidth = NewMaxWidth - MinWidth;
	ActualHeight = NewMaxHeight - MinHeight;
}

void AGenerateMaze::MazeInitialize()
{
	// The maze size must be odd.
	SetMazeSize();

	if (!Math.IsOdd(ActualWidth))
	{
		ActualWidth += 1;
	}
	if (!Math.IsOdd(ActualHeight))
	{
		ActualHeight += 1;
	}

	Maze.Init(ID::Wall, ActualWidth * ActualHeight);

	Math = MyMath(ActualWidth, ActualHeight);

	UE_LOG(LogTemp, Log, TEXT("MAZE INITIALIZE"));
}

void AGenerateMaze::GenerateMaze()
{
	MazeInitialize();

	UE_LOG(LogTemp, Log, TEXT("START GENERATE MAZE"));

	SetOuterWallOfMaze(ID::Path);

	StartDig();

	SetOuterWallOfMaze(ID::Wall);

	//DebugMaze();

	UE_LOG(LogTemp, Log, TEXT("FINISH GENERATE MAZE"));
}

///////////////////////////////////////////
// Generator

bool AGenerateMaze::IsDigAbleRight(const uint16 PosX, const uint16 PosY)
{
	// Check Right
	uint32 Pos = 0, Pos2 = 0;
	if (Math.IsInWidth(PosX + 2) == true && Math.IsInHeight(PosY) == true)
	{
		Pos = Math.GetPos(PosX + 2, PosY);
	}
	else
	{
		return false;
	}

	if (Math.IsInWidth(PosX + 1) == true && Math.IsInHeight(PosY) == true)
	{
		Pos2 = Math.GetPos(PosX + 1, PosY);
	}
	else
	{
		return false;
	}
	return ((IsWall(Pos) == true) && (IsWall(Pos2) == true));
}

bool AGenerateMaze::IsDigAbleLeft(const uint16 PosX, const uint16 PosY)
{
	// Check Left
	uint32 Pos = 0, Pos2 = 0;
	if (Math.IsInWidth(PosX - 2) == true && Math.IsInHeight(PosY) == true)
	{
		Pos = Math.GetPos(PosX - 2, PosY);
	}
	else
	{
		return false;
	}

	if (Math.IsInWidth(PosX - 1) == true && Math.IsInHeight(PosY) == true)
	{
		Pos2 = Math.GetPos(PosX - 1, PosY);
	}
	else
	{
		return false;
	}
	return ((IsWall(Pos) == true) && (IsWall(Pos2) == true));
}

bool AGenerateMaze::IsDigAbleUpper(const uint16 PosX, const uint16 PosY)
{
	// Check Up
	uint32 Pos = 0, Pos2 = 0;
	if (Math.IsInWidth(PosX) == true && Math.IsInHeight(PosY - 2) == true)
	{
		Pos = Math.GetPos(PosX, PosY - 2);
	}
	else
	{
		return false;
	}

	if (Math.IsInWidth(PosX) == true && Math.IsInHeight(PosY - 1) == true)
	{
		Pos2 = Math.GetPos(PosX, PosY - 1);
	}
	else
	{
		return false;
	}
	return ((IsWall(Pos) == true) && (IsWall(Pos2) == true));
}

bool AGenerateMaze::IsDigAbleDown(const uint16 PosX, const uint16 PosY)
{
	// Check Down
	uint32 Pos = 0, Pos2 = 0;
	if (Math.IsInWidth(PosX) == true && Math.IsInHeight(PosY + 2) == true)
	{
		Pos = Math.GetPos(PosX, PosY + 2);
	}
	else
	{
		return false;
	}

	if (Math.IsInWidth(PosX) == true && Math.IsInHeight(PosY + 1) == true)
	{
		Pos2 = Math.GetPos(PosX, PosY + 1);
	}
	else
	{
		return false;
	}
	return ((IsWall(Pos) == true) && (IsWall(Pos2) == true));
}

void AGenerateMaze::StartDig()
{
	while (true)
	{
		Dirs = {};
		if (IsDigAbleLeft(StartX, StartY) == true) Dirs.Add(Direction::Left);
		if (IsDigAbleRight(StartX, StartY) == true) Dirs.Add(Direction::Right);
		if (IsDigAbleUpper(StartX, StartY) == true) Dirs.Add(Direction::Up);
		if (IsDigAbleDown(StartX, StartY) == true) Dirs.Add(Direction::Down);

		int DirsSize = Dirs.Num();
		if (DirsSize == 0) break;

		SetReachPos(StartX, StartY);

		int DigIndex = FMath::RandRange(0, DirsSize - 1);

		Direction NextDig = Dirs[DigIndex];
		if (NextDig == Direction::Left)
		{
			SetReachPos(StartX - 1, StartY);
			SetReachPos(StartX - 2, StartY);
		}
		else if (NextDig == Direction::Right)
		{
			SetReachPos(StartX + 1, StartY);
			SetReachPos(StartX + 2, StartY);
		}
		else if (NextDig == Direction::Up)
		{
			SetReachPos(StartX, StartY - 1);
			SetReachPos(StartX, StartY - 2);
		}
		else if (NextDig == Direction::Down)
		{
			SetReachPos(StartX, StartY + 1);
			SetReachPos(StartX, StartY + 2);
		}
	}

	if (GetDigStartPos() == true)
	{
		StartDig();
	}
}


void AGenerateMaze::SetReachPos(const uint16 PosX, const uint16 PosY)
{
	uint32 Pos = 0;
	if (Math.IsInWidth(PosX) == true && Math.IsInHeight(PosY) == true)
	{
		Pos = Math.GetPos(PosX, PosY);
	}
	else
	{
		return;
	}

	if (Maze.IsValidIndex(Pos) == true)
	{
		Maze[Pos] = ID::Path;
	}

	if ((Math.IsOdd(PosX) == true) && (Math.IsOdd(PosY) == true))
	{
		PathPoses.Add(PathPos(PosX, PosY));
	}
}


bool AGenerateMaze::GetDigStartPos()
{
	int PosSize = PathPoses.Num();
	if (PosSize == 0) return false;

	int Index = FMath::RandRange(0, PosSize - 1);

	Generator->GetItem(PathPoses[Index], StartX, StartY);
	PathPoses.RemoveAt(Index, 1, true);

	return true;
}


bool AGenerateMaze::IsWall(const uint32 Pos)
{
	return Generator->IDIs(Maze, Pos, ID::Wall);
}


///////////////////////////////////////////
// 

void AGenerateMaze::SetOuterWallOfMaze(const ID NewId)
{
	Generator->SetID(NewId, Maze, 0, ActualHeight, 0, false, true, ActualWidth - 1);
	Generator->SetID(NewId, Maze, 0, ActualWidth, 0, true, true, ActualHeight - 1);
}


void AGenerateMaze::DebugMaze()
{
	uint32 CheckIndex = 0;
	FString tmp = "";
	for (int MatY = 0; MatY < ActualHeight; ++MatY)
	{
		tmp = "";
		for (int MatX = 0; MatX < ActualWidth; ++MatX)
		{
			CheckIndex = Math.GetPos(MatX, MatY);
			if (Maze.IsValidIndex(CheckIndex) == true)
			{
				ID id = Maze[CheckIndex];
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


///////////////////////////////////////////
// Getter

void AGenerateMaze::GetActualWidth(uint16& OutWidth)
{
	OutWidth = ActualWidth;
}


void AGenerateMaze::GetActualHeight(uint16& OutHeight)
{
	OutHeight = ActualHeight;
}

///////////////////////////////////////////
// Setter

void AGenerateMaze::SetMaxSize(const uint16 InWidth, const uint16 InHeight)
{
	MaxWidth = InWidth;
	MaxHeight = InHeight;
}
