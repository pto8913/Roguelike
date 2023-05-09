// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/UMG/MapWidget.h"
#include "Public/Dungeon/GenerateExterior.h"
#include "Public/Dungeon/GenerateMaze.h"
#include "Public/RoguelikeGameInstance.h"
#include "Public/RoguelikeGameMode.h"

#include "Button.h"

void UMapWidget::Init()
{
	SizeMaster = GameInstance->GetSizeMaster();
	SizeMastering = SizeMaster / 100;

	TextureData = {};
	MyRGB.Black();
	Markers = {};
	Dungeon = {};
}


void UMapWidget::OpenMap()
{
	if (!IsInViewport())
	{
		GameInstance = URoguelikeGameInstance::GetGameInstance();
		if (GameInstance != nullptr)
		{
			AddToViewport();
			AddDelegate();
			GameInstance->GamePause(FInputModeUIOnly());

			Init();

			SetImage();
		}
	}
}


void UMapWidget::CloseMap()
{
	if (IsInViewport() == true)
	{
		if (GameInstance != nullptr)
		{
			RemoveFromParent();
			RemoveDelegate();
			GameInstance->GameUnPause(FInputModeGameOnly());
		}
	}
}


void UMapWidget::ClickedMark()
{
	SetTextureCurrentLocation(true);
}


void UMapWidget::SetImage()
{
	if (GameInstance != nullptr)
	{
		if (ARoguelikeGameMode::GetGameMode() != nullptr)
		{
			Dungeon = GameInstance->GetDungeon();
			GameInstance->GetDungeonSize(ActualWidth, ActualHeight);
			Math = MyMath(SizeMaster, ActualWidth, ActualHeight);
			SetTexture(Dungeon);
		}
	}

	MapImage->SetBrushFromTexture(MapTexture);
}


void UMapWidget::SetTexture(const TArray<ID> Target)
{
	MapTexture = UTexture2D::CreateTransient(ActualWidth, ActualHeight);
	MapTexture->AddToRoot();
	MapTexture->UpdateResource();

	MyRGB.Black();

	TextureData.Init(MyRGB, ActualHeight * ActualWidth);

	MyRGB.White();
	uint32 Pos = 0;
	for (int y = ActualHeight - 1; y > -1; --y)
	{
		for (int x = 0; x < ActualWidth; ++x)
		{
			Pos = Math.GetPos(x, y);
			if (Target.IsValidIndex(Pos) == true)
			{
				if (Target[Pos] == ID::Wall)
				{
					TextureData[Pos] = MyRGB;
				}
			}
		}
	}

	SetTextureCurrentLocation();

	UpdateMarkers();

	UpdateTexture();
}

void UMapWidget::SetTextureCurrentLocation(const bool bIsSetMark)
{
	GetCurrentLocation();

	MyRGB.Green();

	uint32 Pos = Math.GetPos(CurrentPosX, CurrentPosY);
	if (TextureData.IsValidIndex(Pos) == true)
	{
		if (bIsSetMark == true)
		{
			UpdateMarkers(CurrentPosX, CurrentPosY);
		}
		else
		{
			TextureData[Pos] = MyRGB;
			PrePos = Pos;
		}
		UpdateTexture();
	}
}


void UMapWidget::UpdateMarkers(const uint16 X, const uint16 Y)
{
	if (X != NULL && Y != NULL)
	{
		uint32 MarkersSize = Markers.Num();
		MarkPos NewMark = MarkPos(X, Y);
		if (IsDistinctMark(NewMark) == true) return;

		if (MarkersSize < 6)
		{
			Markers.Add(NewMark);
		}
		else
		{
			if (Markers.IsValidIndex(0) == true)
			{
				Markers.RemoveAt(0);
				Markers.Add(NewMark);
			}
		}
	}

	MyRGB.Yellow();
	uint16 NX = 0, NY = 0;
	for (const MarkPos& Pos : Markers)
	{
		NX = Pos.Get<0>(); NY = Pos.Get<1>();
		uint32 NPos = Math.GetPos(NX, NY);
		if (TextureData.IsValidIndex(NPos) == true)
		{
			TextureData[NPos] = MyRGB;
		}
	}
}


bool UMapWidget::IsDistinctMark(const MarkPos NewMark)
{
	for (const MarkPos& Pos : Markers)
	{
		if (Pos == NewMark)
		{
			return true;
		}
	}
	return false;
}


void UMapWidget::UpdateTexture()
{
	auto locked_bulk_data = MapTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(locked_bulk_data, TextureData.GetData(), sizeof(RGB) * TextureData.Num());
	MapTexture->PlatformData->Mips[0].BulkData.Unlock();
	MapTexture->UpdateResource();
}


/////////////////////////////////////////////////////////////////////
//

void UMapWidget::AddDelegate()
{
	ButtonMark->OnClicked.AddDynamic(this, &UMapWidget::ClickedMark);
	ButtonClose->OnClicked.AddDynamic(this, &UMapWidget::CloseMap);
}


void UMapWidget::RemoveDelegate()
{
	ButtonMark->OnClicked.RemoveDynamic(this, &UMapWidget::ClickedMark);
	ButtonClose->OnClicked.RemoveDynamic(this, &UMapWidget::CloseMap);
}


void UMapWidget::GetCurrentLocation()
{
	CurrentLocation = GameInstance->GetPlayer()->GetActorLocation();

	CurrentPosX = Math.GetGridPosFromLocationPos(CurrentLocation.X, true);
	CurrentPosY = Math.GetGridPosFromLocationPos(CurrentLocation.Y, false);
}


URoguelikeGameInstance* UMapWidget::GetGameInstance()
{
	return GameInstance = URoguelikeGameInstance::GetGameInstance();
}

