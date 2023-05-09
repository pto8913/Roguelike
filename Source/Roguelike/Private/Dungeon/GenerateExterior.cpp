// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Dungeon/GenerateExterior.h"
#include "Public/Player/RoguelikeCharacter.h"
#include "Public/UMG/NotifierWidget.h"
#include "Public/RoguelikeGameMode.h"
#include "Public/MazeGameMode.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine.h"

AGenerateExterior::AGenerateExterior()
{
	GenerateDungeon = CreateDefaultSubobject<AGenerateDungeon>(TEXT("Dungeon"));
	GenerateMaze = CreateDefaultSubobject<AGenerateMaze>(TEXT("Maze"));

	Generator = CreateDefaultSubobject<UptoGenerator>(TEXT("Generator"));

	ConstructorHelpers::FObjectFinder<UClass> TorchFinder(TEXT("Blueprint'/Game/Pack/Blueprint/BP_Torch.BP_Torch_C'"));
	TorchComp = TorchFinder.Object;
}

void AGenerateExterior::Initialize()
{
	OpenType = GameInstance->GetOpenLevelType();
	Result = {};
	PlayerStartCandidate = {};

	Options = GameInstance->GetOptions();

	RoguelikeGameMode = ARoguelikeGameMode::GetGameMode();
	MazeGameMode = AMazeGameMode::GetGameMode();
}

void AGenerateExterior::GenerateExterior()
{
	if (GetGameInstance() != nullptr)
	{
		Initialize();

		if (RoguelikeGameMode != nullptr)
		{
			GenerateDungeonExterior();
		}
		else if (MazeGameMode != nullptr)
		{
			GenerateMazeExterior();
		}
		if (RoguelikeGameMode != nullptr && OpenType != EOpenLevelType::Load)
		{
			GameInstance->SpawnEnemy();
		}
	}
}

void AGenerateExterior::GenerateDungeonExterior()
{
	if (GetGameInstance() == nullptr) return;
	if (GenerateDungeon == nullptr) return;

	if (OpenType != EOpenLevelType::Load)
	{
		GenerateDungeon->GenerateDungeon();

		GenerateDungeon->GetActualWidth(ActualWidth);
		GenerateDungeon->GetActualHeight(ActualHeight);

		Math = MyMath(SizeMaster, ActualWidth, ActualHeight);

		Result = GenerateDungeon->GetDungeon();

		GameInstance->SetDungeon(Result);
		GameInstance->SetDungeonSize(ActualWidth, ActualHeight);
	}
	else if (OpenType == EOpenLevelType::Load)
	{
		Result = GameInstance->GetDungeon();
		GameInstance->GetDungeonSize(ActualWidth, ActualHeight);
		Math = MyMath(SizeMaster, ActualWidth, ActualHeight);
	}

	GameInstance->SetMasterSize(SizeMaster);
	SetFloorSize();

	uint32 Pos = 0;
	bool bIsSetTorch = false;
	TArray<Direction> Dirs = {};
	for (uint16 PosX = 0; PosX < ActualWidth; ++PosX)
	{
		for (uint16 PosY = 0; PosY < ActualHeight; ++PosY)
		{
			Pos = Math.GetPos(PosX, PosY);
			Dirs = {};
			HorizonLocation = Math.GetWorldHorizontalLocation(PosX);
			VerticalLocation = Math.GetWorldVerticalLocation(PosY);

			CheckCrack(PosX, PosY);

			if (Generator->IDIs(Result, Pos, ID::Wall) == true)
			{
				if (Pos % 3 == 0) bIsSetTorch = true;
				else bIsSetTorch = false;

				CheckCrack(Dirs, PosX, PosY);
				GenerateWall("Dungeon", Dirs, bIsSetTorch);
			}
			else if (Generator->IDIs(Result, Pos, ID::Room) == true)
			{
				PlayerStartCandidate.Add({
					HorizonLocation,
					VerticalLocation,
					10.f
				});
				if (Options.GraphicType == EGraphicType::High)
				{
					GenerateFloor("Dungeon", true, 0, 0);
				}
				else if (Options.GraphicType == EGraphicType::Mid)
				{
					GenerateFloor("Dungeon", false, 0, 0);
				}
			}
			else if (Generator->IDIs(Result, Pos, ID::Path, ID::EndOfPath) == true)
			{
				if (Options.GraphicType == EGraphicType::High)
				{
					GenerateFloor("Dungeon", true, 0, 0);
				}
				else if (Options.GraphicType == EGraphicType::Mid)
				{
					GenerateFloor("Dungeon", false, 0, 0);
				}
			}
		}
	}

	if (Options.GraphicType == EGraphicType::Mid)
	{
		// Roof is low quality
		GenerateFloor("Dungeon", true, FVector(FloorWidth, FloorHeight, 10.f), FVector(0.f, 0.f, 1000.f));
	}
	else if (Options.GraphicType == EGraphicType::Low)
	{
		/* Roof and Floor set low quality */
		GenerateFloor("Dungeon", false, FVector(FloorWidth, FloorHeight, 10.f), FVector(0.f, 0.f, -500.f));
		GenerateFloor("Dungeon", true, FVector(FloorWidth, FloorHeight, 10.f), FVector(0.f, 0.f, 1000.f));
	}

	bool bIsSpawnPortal = false;
	if (OpenType != EOpenLevelType::Load)
	{
		GetStartCandidate(true);

		bIsSpawnPortal = GenerateDungeon->IsSpawnPortal();
		GameInstance->SetIsSpawnPortal(bIsSpawnPortal);
	}
	else if (OpenType == EOpenLevelType::Load)
	{
		bIsSpawnPortal = GameInstance->GetIsSpawnPortal();
	}

	if (bIsSpawnPortal == true)
	{
		SpawnPortal(EPortalType::IntoMaze);
	}
	SpawnStair(EPortalType::NextFloor);

	GameInstance->GetPlayer()->SetActorLocation(GameInstance->GetDungeonPlayerStartLocation());
}

void AGenerateExterior::GenerateMazeExterior()
{
	if (GenerateMaze == nullptr) return;

	if (OpenType != EOpenLevelType::Load)
	{
		GenerateMaze->GenerateMaze();

		GenerateMaze->GetActualWidth(ActualWidth);
		GenerateMaze->GetActualHeight(ActualHeight);

		Math = MyMath(SizeMaster, ActualWidth, ActualHeight);

		Result = GenerateMaze->GetMaze();

		GameInstance->SetMaze(Result);
		GameInstance->SetMazeSize(ActualWidth, ActualHeight);
	}
	else if (OpenType == EOpenLevelType::Load)
	{
		Result = GameInstance->GetMaze();
		GameInstance->GetMazeSize(ActualWidth, ActualHeight);
		Math = MyMath(SizeMaster, ActualWidth, ActualHeight);
	}

	GameInstance->SetMasterSize(SizeMaster);
	SetFloorSize();

	uint32 Pos = 0;
	bool bIsSetTorch = false;
	TArray<Direction> Dirs = {};
	for (uint16 PosX = 0; PosX < ActualWidth; ++PosX)
	{
		for (uint16 PosY = 0; PosY < ActualHeight; ++PosY)
		{
			Pos = Math.GetPos(PosX, PosY);
			Dirs = {};
			HorizonLocation = Math.GetWorldHorizontalLocation(PosX);
			VerticalLocation = Math.GetWorldVerticalLocation(PosY);
			if (Generator->IDIs(Result, Pos, ID::Wall) == true)
			{
				if (Pos % 3 == 0) bIsSetTorch = true;
				else bIsSetTorch = false;

				CheckCrack(Dirs, PosX, PosY);
				GenerateWall("Maze", Dirs, bIsSetTorch);
			}
			else if (Generator->IDIs(Result, Pos, ID::Path) == true)
			{
				PlayerStartCandidate.Add({
					HorizonLocation,
					VerticalLocation,
					10.f
				});
				if (Options.GraphicType == EGraphicType::High)
				{
					GenerateFloor("Maze", true, 0, 0);
				}
				else if (Options.GraphicType == EGraphicType::Mid)
				{
					GenerateFloor("Maze", false, 0, 0);
				}
			}
		}
	}

	if (Options.GraphicType == EGraphicType::Mid)
	{
		/* make low quality roof */
		GenerateFloor("Maze", true, FVector(FloorWidth, FloorHeight, 10.f), FVector(0.f, 0.f, 1000.f));
	}
	else if (Options.GraphicType == EGraphicType::Low)
	{
		/* make low quality roof and floor */
		GenerateFloor("Maze", false, FVector(FloorWidth, FloorHeight, 10.f), FVector(0.f, 0.f, -500.f));
		GenerateFloor("Maze", true, FVector(FloorWidth, FloorHeight, 10.f), FVector(0.f, 0.f, 1000.f));
	}
	
	GetStartCandidate(false);
	SpawnPortal(EPortalType::GetOutMaze);

	GameInstance->GetPlayer()->SetActorLocation(GameInstance->GetMazePlayerStartLocation());
}

//////////////////////////////////////////////////////////////////
// Make Wall

void AGenerateExterior::GenerateWall(const FName Tag, const TArray<Direction> Dirs, const bool bIsSetTorch)
{
	if (CheckWorld() == nullptr) return;

	uint32 DirsSize = Dirs.Num();
	if (DirsSize == 0) return;
	if (DirsSize >= 2)
	{
		return GenerateWallWithTag(Tag, bIsSetTorch, Dirs[0]);
	}
	else
	{
		return GenerateWall(Tag, bIsSetTorch, Dirs[0]);
	}
}

void AGenerateExterior::GenerateWall(FName Tag, bool bIsSetTorch, Direction WalkableDir)
{
	if (CheckWorld() == nullptr) return;
	if (WalkableDir == Direction::None) return;

	float LocX = HorizonLocation, LocY = VerticalLocation, LocZ = AdjLoc;

	Trans = {
		FRotator::ZeroRotator,
		FVector(LocX, LocY, LocZ),
		FVector(SizeMastering, SizeMastering, SizeMastering)
	};

	ExtWall = world->SpawnActorDeferred<ADungeonExterior>(
		ADungeonExterior::StaticClass(),
		Trans
		);

	ExtWall->SetWallMaterial();
	ExtWall->Tags.Add(Tag);

	ExtWall->FinishSpawning(Trans);

	if (bIsSetTorch == true)
	{
		FRotator Rot = FRotator::ZeroRotator;
		Trans = FTransform::Identity;

		Torch = world->SpawnActorDeferred<AActor>(TorchComp, Trans);

		if (WalkableDir == Direction::Up)
		{
			Rot.Yaw += 180.f;
			LocY -= AdjLoc;
		}
		if (WalkableDir == Direction::Down)
		{
			LocY += AdjLoc;
		}
		if (WalkableDir == Direction::Left)
		{
			Rot.Yaw -= 90.f;
			LocX += AdjLoc;
		}
		if (WalkableDir == Direction::Right)
		{
			Rot.Yaw += 90.f;
			LocX -= AdjLoc;
		}
		Trans.SetLocation({ LocX, LocY, LocZ });
		Trans.SetRotation(FQuat(Rot));
		Trans.SetScale3D({ 1.f, 1.f, 1.f });

		ExtWall->CompAttachToMesh(Torch);
		Torch->Tags.Add(Tag);
		Torch->FinishSpawning(Trans);
	}
}

void AGenerateExterior::GenerateWallWithTag(FName Tag, bool bIsSetTorch, Direction WalkableDir)
{
	if (CheckWorld() == nullptr) return;

	float LocX = HorizonLocation, LocY = VerticalLocation, LocZ = AdjLoc;

	Trans = {
		FRotator::ZeroRotator,
		{LocX, LocY, LocZ},
		{SizeMastering, SizeMastering, SizeMastering}
	};

	ExtWall = world->SpawnActorDeferred<ADungeonExterior>(
		ADungeonExterior::StaticClass(),
		Trans
		);

	ExtWall->SetWallMaterial();
	ExtWall->Tags.Add(Tag);

	ExtWall->FinishSpawning(Trans);

	if (bIsSetTorch == true)
	{
		FRotator Rot = FRotator::ZeroRotator;
		Trans = FTransform::Identity;

		Torch = world->SpawnActorDeferred<AActor>(TorchComp, Trans);

		if (WalkableDir == Direction::Up)
		{
			Rot.Yaw += 180.f;
			LocY -= AdjLoc;
		}
		if (WalkableDir == Direction::Down)
		{
			LocY += AdjLoc;
		}
		if (WalkableDir == Direction::Left)
		{
			Rot.Yaw -= 90.f;
			LocX += AdjLoc;
		}
		if (WalkableDir == Direction::Right)
		{
			Rot.Yaw += 90.f;
			LocX -= AdjLoc;
		}
		Trans.SetLocation({ LocX, LocY, LocZ });
		Trans.SetRotation(FQuat(Rot));
		Trans.SetScale3D({ 1.f, 1.f, 1.f });

		ExtWall->CompAttachToMesh(Torch);
		Torch->Tags.Add(Tag);
		Torch->FinishSpawning(Trans);
	}
}

void AGenerateExterior::CheckCrack(TArray<Direction>& Dirs, const uint16 PosX, const uint16 PosY)
{
	int32 Pos = 0;
	for (uint8 Idx = 0; Idx < 4; ++Idx)
	{
		Pos = Math.GetPos(PosX + dx[Idx], PosY + dy[Idx]);
		if (Generator->IDIs(Result, Pos, ID::Room, ID::Path, ID::EndOfPath) == true)
		{
			if (Idx == 0) Dirs.Add(Direction::Up);
			if (Idx == 1) Dirs.Add(Direction::Down);
			if (Idx == 2) Dirs.Add(Direction::Left);
			if (Idx == 3) Dirs.Add(Direction::Right);
		}
	}
}

void AGenerateExterior::CheckCrack(const uint16 PosX, const uint16 PosY)
{
	int32 Pos = Math.GetPos(PosX, PosY);
	uint8 Count = 0;
	if (Generator->IDIs(Result, Pos, ID::Path, ID::Room, ID::EndOfPath) == true) return;

	for (uint8 Idx = 0; Idx < 4; ++Idx)
	{
		Pos = Math.GetPos(PosX + dx[Idx], PosY + dy[Idx]);
		if (Generator->IDIs(Result, Pos, ID::Wall) == true)
		{
			++Count;
		}
	}
	if (Count > 1)
	{
		GenerateWallWithTag("Dungeon");
	}
}


///////////////////////////////////////
// Make Floor

void AGenerateExterior::GenerateFloor(
	FName Tag, bool bIsMakeRoof,
	FVector InScale, FVector InLocation
)
{
	if (CheckWorld() == nullptr) return;

	Trans = {
		FRotator::ZeroRotator,
		FVector::ZeroVector,
		InScale
	};

	ExtFloor = world->SpawnActorDeferred<ADungeonExterior>(
		ADungeonExterior::StaticClass(), Trans
		);

	ExtFloor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);

	ExtFloor->Tags.Add(Tag);
	ExtFloor->SetFloorMaterial();

	if (bIsMakeRoof == true)
	{
		if (InLocation != FVector::ZeroVector)
		{
			Trans.SetLocation(InLocation);
		}
		else
		{
			Trans.SetLocation(FVector(0.f, 0.f, 500.f));
		}
	}
	else
	{
		if (InLocation != FVector::ZeroVector)
		{
			Trans.SetLocation(InLocation);
		}
		else
		{
			Trans.SetLocation(FVector(0.f, 0.f, 0.f));
		}
	}

	ExtFloor->FinishSpawning(Trans);
}

void AGenerateExterior::GenerateFloor(
	FName Tag, bool bIsMakeRoof,
	uint16 PosX, uint16 PosY
)
{
	if (CheckWorld() == nullptr) return;

	Trans = {
		FRotator::ZeroRotator,
		FVector(HorizonLocation, VerticalLocation, -AdjLoc),
		{10.f, 10.f, SizeMastering}
	};

	ExtFloor = world->SpawnActorDeferred<ADungeonExterior>(
		ADungeonExterior::StaticClass(), Trans
		);

	ExtFloor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);

	ExtFloor->Tags.Add(Tag);

	ExtFloor->SetFloorMaterial();

	ExtFloor->FinishSpawning(Trans);

	if (bIsMakeRoof == true)
	{
		Trans = {
			FRotator::ZeroRotator,
			FVector(HorizonLocation, VerticalLocation, 500.f),
			FVector(5.f, 5.f, 0.1f)
		};

		ExtRoof = world->SpawnActorDeferred<ADungeonExterior>(
			ADungeonExterior::StaticClass(),
			Trans
			);

		ExtRoof->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);

		ExtRoof->Tags.Add(Tag);

		ExtRoof->SetFloorMaterial();
		ExtRoof->FinishSpawning(Trans);
	}

}


///////////////////////////////////////
// Make Portal

void AGenerateExterior::SpawnPortal(EPortalType InType)
{
	if (CheckWorld() == nullptr) return;
	if (GetGameInstance() == nullptr) return;

	Trans = {
		FRotator::ZeroRotator,
		FVector::ZeroVector,
		{ 1.f, 1.f, 1.f }
	};

	ADungeonPortal* Portal = nullptr;
	if (InType == EPortalType::IntoMaze)
	{
		//NotifierWidget = GameInstance->CreateCustomWidget<UNotifierWidget>(GameInstance->GetNotifierUIPath());
		NotifierWidget = GameInstance->GetNotifierUI();
		if (NotifierWidget != nullptr)
		{
			NotifierWidget->OpenNotifier();
		}

		if (OpenType != EOpenLevelType::Load)
		{
			// Create a portal where the ID is EndOfPath
			uint16 PosX = 0, PosY = 0;
			PathPos Pos = GenerateDungeon->GetRndEndPos();
			Generator->GetItem(Pos, PosX, PosY);
			HorizonLocation = Math.GetWorldHorizontalLocation(PosX);
			VerticalLocation = Math.GetWorldVerticalLocation(PosY);
			Trans.SetLocation({ HorizonLocation, VerticalLocation, 10.f });
		}
		else
		{
			// when OpenType is Load, Get dungeon player end location
			Trans.SetLocation(GameInstance->GetPortalLocation());
		}
	}
	else if (InType == EPortalType::GetOutMaze)
	{
		// Get of maze player end location
		Trans.SetLocation(GameInstance->GetMazePlayerEndLocation());
	}

	Portal = world->SpawnActorDeferred<ADungeonPortal>(
		ADungeonPortal::StaticClass(),
		Trans
		);

	Portal->ParticleSysComponent->SetTemplate(Portal->GateParticle);

	GameInstance->SetPortalType(InType);
	GameInstance->SetPortalLocation(Trans.GetLocation());

	Portal->FinishSpawning(Trans);
}

void AGenerateExterior::SpawnStair(EPortalType InType)
{
	if (GetGameInstance() == nullptr) return;

	Trans = {
		FRotator::ZeroRotator,
		FVector::ZeroVector,
		{ 1.f, 1.f, 1.f }
	};

	AStair* Stair = nullptr;
	if (InType == EPortalType::NextFloor)
	{
		if (OpenType != EOpenLevelType::Load)
		{
			int32 Index = GetRndStartCandidateIndex();
			if (Index == -1) return;

			Trans.SetLocation(PlayerStartCandidate[Index]);
		}
		else
		{
			Trans.SetLocation(GameInstance->GetDungeonPlayerEndLocation());
		}
	}

	Stair = world->SpawnActorDeferred<AStair>(
		AStair::StaticClass(),
		Trans
		);

	GameInstance->SetStairType(InType);

	Stair->FinishSpawning(Trans);
}

/////////////////////////////////////////////////////////////////
// StartPoint

void AGenerateExterior::GetStartCandidate(bool bIsDungeon)
{
	int32 Index = GetRndStartCandidateIndex();
	if (Index == -1) return;

	if (bIsDungeon == true)
	{
		GameInstance->SetDungeonPlayerStartLocation(PlayerStartCandidate[Index]);
	}
	else
	{
		GameInstance->SetMazePlayerStartLocation(PlayerStartCandidate[Index]);
	}

	PlayerStartCandidate.RemoveAt(Index);

	GetEndCandidate(bIsDungeon);
}

void AGenerateExterior::GetEndCandidate(bool bIsDungeon)
{
	int32 Index = GetRndStartCandidateIndex();
	if (Index == -1) return;

	if (bIsDungeon == true)
	{
		GameInstance->SetDungeonPlayerEndLocation(PlayerStartCandidate[Index]);
	}
	else
	{
		GameInstance->SetMazePlayerEndLocation(PlayerStartCandidate[Index]);
	}

	PlayerStartCandidate.RemoveAt(Index);

	GameInstance->SetEnemyLoiterPos(PlayerStartCandidate);
}

int32 AGenerateExterior::GetRndStartCandidateIndex()
{
	StartCandidateSize = PlayerStartCandidate.Num();
	if (StartCandidateSize == 0) return -1;

	return FMath::RandRange(0, StartCandidateSize - 1);
}


////////////////////////////////////////////////////
// Templates

URoguelikeGameInstance* AGenerateExterior::GetGameInstance()
{
	return GameInstance = URoguelikeGameInstance::GetGameInstance();
}

void AGenerateExterior::GetActualSize()
{
	ActualHeight = MaxHeight - MinHeight;
	ActualWidth = MaxWidth - MinWidth;
}

UWorld* AGenerateExterior::CheckWorld()
{
	world = nullptr;
	world = GEngine->GameViewport->GetWorld();
	if (world == nullptr)
	{
		world = GetWorld();
	}
	return world;
}

void AGenerateExterior::SetFloorSize()
{
	FloorWidth = ActualWidth * SizeMastering;
	FloorHeight = ActualHeight * SizeMastering;
}