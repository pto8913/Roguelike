// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Templates/MyMath.h"

float MyMath::GetGridPosFromLocationPos(const float Pos, const bool bIsX)
{
	float Result = 0;
	//UE_LOG(LogTemp, Log, TEXT("GetGridPosFromLocationPos %.2f"), Pos);

	if (bIsX == true)
	{
		if (Pos > 0)
		{
			Result = ActualWidth / 2 - Pos / SizeMaster;
		}
		else
		{
			Result = ActualWidth - WorldRadius(ActualWidth + 1, FMath::Abs(Pos)) / SizeMaster - 1.f;
		}
	}
	else
	{
		if (Pos > 0)
		{
			Result = ActualHeight / 2 - Pos / SizeMaster;
		}
		else
		{
			Result = ActualHeight - WorldRadius(ActualHeight + 1, FMath::Abs(Pos)) / SizeMaster - 1.f;
		}
	}
	return Result;
}

float MyMath::GetWorldHorizontalLocation(const uint16 PosX)
{
	return -(PosX - GridRadius(FloorWidth)) * SizeMaster;
}

float MyMath::GetWorldVerticalLocation(const uint16 PosY)
{
	return -(PosY - GridRadius(FloorHeight)) * SizeMaster;
}

/////////////////////////////////////////
//

uint32 MyMath::GetPos(const uint16 PosX, const uint16 PosY)
{
	return PosY * ActualWidth + PosX;
}


bool MyMath::IsNotOuterWall(const uint32 Pos)
{
	if ((Pos == 0) || (Pos == ActualWidth) || (Pos == ActualHeight)) return false;
	return true;
}

bool MyMath::IsOdd(const uint16 InValue)
{
	if (InValue % 2 == 0) return false;
	return true;
}

bool MyMath::IsInWidth(const uint16 PosX)
{
	if (PosX != 0 && PosX != ActualWidth - 1) return true;
	return false;
}


bool MyMath::IsInHeight(const uint16 PosY)
{
	if (PosY != 0 && PosY != ActualHeight - 1) return true;
	return false;
}

/////////////////////////////////////////
//

uint16 MyMath::WorldRadius(const uint16 Size, const float Pos)
{
	return FMath::Pow(Size, 2) * SizeMastering - SizeMaster / 2 - Pos;
}


float MyMath::GridRadius(const float Size)
{
	return ((Size - SizeMastering * 2) / SizeMastering + 1) / 2;
}


/////////////////////////////////////////
//

void MyMath::SetActualSize(const uint16 Width, const uint16 Heihgt)
{
	ActualWidth = Width;
	ActualHeight = Heihgt;
}

void MyMath::SetMasterSize(const uint16 Size)
{
	SizeMaster = Size;
	SizeMastering = Size / 100;
}

void MyMath::SetFloorSize()
{
	FloorWidth = ActualWidth * SizeMastering;
	FloorHeight = ActualHeight * SizeMastering;
}