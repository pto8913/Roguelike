// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/Templates/MyMath.h"
#include "Templates/Tuple.h"
#include "Public/Templates/ptoEnums.h"
#include "ptoGenerator.generated.h"

#define PathPos TTuple<uint16, uint16>
#define RECTS TTuple<uint16, uint16, uint16, uint16>

UCLASS()
class ROGUELIKE_API UptoGenerator : public UObject
{
	GENERATED_BODY()
private:
	MyMath Math = MyMath();
public:

	UptoGenerator() {};

	/*
		If you want to make
		"Horizontal line"
		bIsConstantHorizon set true and Constant Y position.

		"Vertical line"
		bIsConstantHorizon set false and Constant X position.

		"Rect"
		You should call this by twice. (for set X or Y)
		bIsMakeRoom set true and ConstantPos2 X or Y position.
	*/
	inline void SetID(
		const ID NewId, TArray<ID>& Target,
		const uint16 MinPos, const uint16 MaxPos, const uint16 ConstantPos,
		const bool bIsConstantHorizon,
		bool bIsMakeRoom = false, uint16 ConstantPos2 = NULL
	);

	inline void SetID(
		const ID NewId, TArray<ID>& Target, TArray<PathPos>& PathPoses,
		const uint16 MinPos, const uint16 MaxPos, const uint16 ConstantPos,
		const bool bIsConstantHorizon
	);

	inline void SetIDPoint(const ID NewId, TArray<ID>& Target, const uint32 Pos);


	inline ID GetID(TArray<ID>& Target, const uint32 Pos);

	inline bool IDIs(const TArray<ID> Target, const uint32 Pos, const ID Id1);
	inline bool IDIs(const TArray<ID> Target, const uint32 Pos, const ID Id1, const ID Id2);
	inline bool IDIs(const TArray<ID> Target, const uint32 Pos, const ID Id1, const ID Id2, const ID Id3);

	inline void GetItem(PathPos Target, uint16& PosX, uint16& PosY);
	inline void GetItem(RECTS Target, uint16& MinX, uint16& MinY, uint16& MaxX, uint16& MaxY);
};
