// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Templates/ptoGenerator.h"

ID UptoGenerator::GetID(TArray<ID>& Target, const uint32 Pos)
{
	if (Target.IsValidIndex(Pos) == true)
	{
		return Target[Pos];
	}
	return ID::None;
}

bool UptoGenerator::IDIs(const TArray<ID> Target, const uint32 Pos, const ID Id1)
{
	if (Target.IsValidIndex(Pos) == true)
	{
		return (Target[Pos] == Id1);
	}
	return false;
}

bool UptoGenerator::IDIs(const TArray<ID> Target, const uint32 Pos, const ID Id1, const ID Id2)
{
	return ((IDIs(Target, Pos, Id1) == true) || (IDIs(Target, Pos, Id2) == true));
}

bool UptoGenerator::IDIs(const TArray<ID> Target, const uint32 Pos, const ID Id1, const ID Id2, const ID Id3)
{
	return ((IDIs(Target, Pos, Id1, Id2) == true) || (IDIs(Target, Pos, Id3) == true));
}

void UptoGenerator::GetItem(PathPos Target, uint16& PosX, uint16& PosY)
{
	PosX = Target.Get<0>();
	PosY = Target.Get<1>();
}

void UptoGenerator::GetItem(
	RECTS Target,
	uint16& MinX, uint16& MinY, uint16& MaxX, uint16& MaxY
)
{
	MinX = Target.Get<0>();
	MinY = Target.Get<1>();
	MaxX = Target.Get<2>();
	MaxY = Target.Get<3>();
}

///////////////////////////////////
//

void UptoGenerator::SetID(
	const ID NewId, TArray<ID>& Target,
	const uint16 MinPos, const uint16 MaxPos, const uint16 ConstantPos,
	const bool bIsConstantHorizon,
	bool bIsMakeRoom, uint16 ConstantPos2
)
{
	uint32 Pos = 0;
	for (uint32 Idx = MinPos; Idx < MaxPos; ++Idx)
	{
		if (bIsConstantHorizon == true)
		{
			Pos = Math.GetPos(Idx, ConstantPos);
		}
		else
		{
			Pos = Math.GetPos(ConstantPos, Idx);
		}

		SetIDPoint(NewId, Target, Pos);

		if (bIsMakeRoom == true)
		{
			if (ConstantPos2 == NULL) continue;

			if (bIsConstantHorizon == true)
			{
				Pos = Math.GetPos(Idx, ConstantPos2);
			}
			else
			{
				Pos = Math.GetPos(ConstantPos2, Idx);
			}

			SetIDPoint(NewId, Target, Pos);
		}
	}
}


void UptoGenerator::SetID(
	const ID NewId, TArray<ID>& Target, TArray<PathPos>& PathPoses,
	const uint16 MinPos, const uint16 MaxPos, const uint16 ConstantPos,
	const bool bIsConstantHorizon
)
{
	uint32 Pos = 0;
	for (uint32 Idx = MinPos; Idx < MaxPos; ++Idx)
	{
		if (bIsConstantHorizon == true)
		{
			Pos = Math.GetPos(Idx, ConstantPos);
			PathPoses.Add(PathPos(Idx, ConstantPos));
		}
		else
		{
			Pos = Math.GetPos(ConstantPos, Idx);
			PathPoses.Add(PathPos(ConstantPos, Idx));
		}

		SetIDPoint(NewId, Target, Pos);

	}
}


void UptoGenerator::SetIDPoint(const ID NewId, TArray<ID>& Target, const uint32 Pos)
{
	if (Target.IsValidIndex(Pos) == true)
	{
		Target[Pos] = NewId;
	}
}

