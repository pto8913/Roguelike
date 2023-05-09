// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "Image.h"
#include "Templates/Tuple.h"
#include "Public/Templates/MyMath.h"
#include "Public/Templates/ptoGenerator.h"
#include "MapWidget.generated.h"

class URoguelikeGameInstance;
class UButton;

struct RGB
{
	void Black()
	{
		R = 0; G = 0; B = 0;
	}

	void Yellow()
	{
		R = 255; G = 255; B = 0;
	}

	void White()
	{
		R = 255; G = 255; B = 255;
	}

	void Red()
	{
		R = 255; G = 0; B = 0;
	}

	void Green()
	{
		R = 0; G = 255; B = 0;
	}

private:
	uint8 R = 0, G = 0, B = 0, Alp = 255;
};

#define MarkPos TTuple<uint16, uint16>

UCLASS()
class ROGUELIKE_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
		ARoguelikeGameMode* GameMode = nullptr;

	UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;
	URoguelikeGameInstance* GetGameInstance();

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonMark = nullptr;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonClose = nullptr;

	UPROPERTY(meta = (BindWidget))
		UImage* MapImage = nullptr;

	UPROPERTY()
		UTexture2D* MapTexture = nullptr;

	UPROPERTY()
		FVector CurrentLocation;

	UPROPERTY()
		uint16 CurrentPosX = 0;

	UPROPERTY()
		uint16 CurrentPosY = 0;

	UPROPERTY()
		uint16 ActualWidth = 0;

	UPROPERTY()
		uint16 ActualHeight = 0;

	UPROPERTY()
		TArray<ID> Dungeon;

	UPROPERTY()
		TArray<ID> Maze;

	UPROPERTY()
		uint16 SizeMaster = 0;

	UPROPERTY()
		uint16 SizeMastering = 0;

	UPROPERTY()
		int32 PrePos = -1;

	MyMath Math = MyMath();

	TArray<MarkPos> Markers;

	void Init();

	void AddDelegate();
	void RemoveDelegate();

	UFUNCTION()
		void ClickedMark();

	RGB MyRGB;
	TArray<RGB> TextureData;
	/* If current mode is Dungeon make dungeon map image */
	void SetImage();
	/* If ID is wall. set white. */
	void SetTexture(const TArray<ID> Target);
	void UpdateTexture();

	/* Get grid position from world position */
	void GetCurrentLocation();
	/* Set green from player grid location */
	void SetTextureCurrentLocation(const bool bIsSetMark = false);
	/* Set mark position. (number of max = 6) */
	void UpdateMarkers(const uint16 X = NULL, const uint16 Y = NULL);
	bool IsDistinctMark(const MarkPos NewMark);
public:
	void OpenMap();

	UFUNCTION()
		void CloseMap();
};