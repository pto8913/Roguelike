// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/RoguelikeGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "MoviePlayer/Public//MoviePlayer.h"

#include "Public/Save/RoguelikeSaveGame.h"
#include "Public/Enemy/EnemyInterface.h"

void URoguelikeGameInstance::Init()
{
	TitleUI = CreateWidget<UTitleWidget>(this, TitleClass);
	NotifierUI = CreateWidget<UNotifierWidget>(this, NotifierClass);
	OptionUI = CreateWidget<UOptionWidget>(this, OptionClass);
	MapUI = CreateWidget<UMapWidget>(this, MapClass);
	PlayerUI = CreateWidget<UPlayerWidget>(this, PlayerClass);
	LoadUI = CreateWidget<ULoadingWidget>(this, LoadClass);
	SelectDataUI = CreateWidget<USelectDataWidget>(this, SelectDataClass);
	ResultUI = CreateWidget<UResultWidget>(this, ResultClass);
	SaveCompleteUI = CreateWidget<UUserWidget>(this, SaveCompleteClass);
	SaveFailedUI = CreateWidget<UUserWidget>(this, SaveFailedClass);
	ScoreUI = CreateWidget<UScoreWidget>(this, ScoreClass);
	TutorialUI = CreateWidget<UTutorialWidget>(this, TutorialClass);
}

void URoguelikeGameInstance::BeginLoadingScreen(const FString& InMapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.WidgetLoadingScreen = LoadUI->TakeWidget();

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
		UGameplayStatics::OpenLevel(CheckWorld(), FName(*InMapName));
	}
}

void URoguelikeGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	if (!IsRunningDedicatedServer())
	{
		if (LoadUI != nullptr)
		{
			LoadUI->CloseLoad();
		}
	}
}

URoguelikeGameInstance* URoguelikeGameInstance::GetGameInstance()
{
	URoguelikeGameInstance* instance = nullptr;

	if (GEngine != nullptr)
	{
		FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		instance = Cast<URoguelikeGameInstance>(context->OwningGameInstance);
	}

	return instance;
}

/////////////////////////////////////////////////////////
// UMG

UEnemyWidget* URoguelikeGameInstance::CreateEnemyUI()
{
	return CreateWidget<UEnemyWidget>(this, EnemyClass);
}

/////////////////////////////////////////////////////////
// 

APlayerController* URoguelikeGameInstance::GetPlayerController()
{
	return Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (GetPlayer() == nullptr) return nullptr;

	AController* tmp = Player->GetController();
	if (tmp == nullptr) return nullptr;

	return Controller = Cast<APlayerController>(tmp);
}

void URoguelikeGameInstance::GamePause(const FInputModeDataBase& InData)
{
	if (GetPlayerController() == nullptr) return;

	Controller->SetPause(true);
	Controller->bShowMouseCursor = true;
	SetInputMode(InData);
}

void URoguelikeGameInstance::GameUnPause(const FInputModeDataBase& InData)
{
	if (GetPlayerController() == nullptr) return;

	Controller->SetPause(false);
	Controller->bShowMouseCursor = false;
	SetInputMode(InData);
}

void URoguelikeGameInstance::SetInputMode(const FInputModeDataBase& InData)
{
	if (GetPlayerController() == nullptr) return;

	Controller->SetInputMode(InData);
}

void URoguelikeGameInstance::SetMouseCursor(bool In)
{
	if (GetPlayerController() == nullptr) return;

	Controller->bShowMouseCursor = In;
}

UWorld* URoguelikeGameInstance::CheckWorld()
{
	world = nullptr;
	world = GEngine->GameViewport->GetWorld();
	if (world == nullptr)
	{
		world = GetWorld();
	}
	return world;
}

/////////////////////////////////////////////////////////
// For save

void URoguelikeGameInstance::SaveGame(
	const FString SlotName, const uint16 Index, const bool bIsWidget, 
	const bool bIsScore, const bool bIsOptions
)
{
	URoguelikeSaveGame* SaveGameInstance = Cast<URoguelikeSaveGame>(UGameplayStatics::CreateSaveGameObject(URoguelikeSaveGame::StaticClass()));
	if (SaveGameInstance == nullptr) return;

	if (bIsOptions == true)
	{
		SaveGameInstance->Options = GetOptions();
	}
	else if (bIsScore == true)
	{
		SaveGameInstance->Score = GetScore();
	}
	else
	{
		DungeonInfos.DungeonPlayerStartLocation = GetPlayer()->GetActorLocation();
		DungeonInfos.MazePlayerStartLocation = GetPlayer()->GetActorLocation();

		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsWithInterface(CheckWorld(), UEnemyInterface::StaticClass(), Actors);
		uint32 Size = Actors.Num();

		DungeonInfos.InitEnemyInfo(Size);

		float FullHealth = 0.f, Health = 0.f;
		for (uint32 Idx = 0; Idx < Size; ++Idx)
		{
			AActor* Actor = Actors[Idx];

			DungeonInfos.EnemyClasses[Idx] = Actor->GetClass();
			DungeonInfos.EnemyNames[Idx] = FName(*Actor->GetName());
			DungeonInfos.EnemyTransforms[Idx] = Actor->GetTransform();

			IEnemyInterface::Execute_ActorSaveDataSaved(Actor, FullHealth, Health);

			DungeonInfos.EnemyFullHealth[Idx] = FullHealth;
			DungeonInfos.EnemyHealth[Idx] = Health;
		}

		if (CheckWorld() != nullptr)
		{
			DungeonInfos.PlayTime += world->GetTimeSeconds();
		}
		
		SaveGameInstance->DungeonInfos = DungeonInfos;
		SaveGameInstance->TimeStamp = FDateTime::Now();
	}
	
	SaveGameInstance->SaveSlotName = SlotName;
	SaveGameInstance->Index = Index;
	bool res = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, Index);
	if (bIsWidget == true)
	{
		if (res == true)
		{
			SaveCompleteUI->AddToViewport();
		}
		else
		{
			SaveFailedUI->AddToViewport();
		}
	}
}

void URoguelikeGameInstance::LoadGame(
	const FString SlotName, const uint16 Index, 
	const bool bIsScore, const bool bIsOptions
)
{
	if (CheckWorld() == nullptr) return;

	URoguelikeSaveGame* SaveGameInstance = Cast<URoguelikeSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, Index));
	if (SaveGameInstance == nullptr) return;

	if (bIsOptions == true)
	{
		SetOptions(SaveGameInstance->Options);
		return;
	}

	if (bIsScore == true)
	{
		SetScore(SaveGameInstance->Score);
		return;
	}

	DungeonInfos = SaveGameInstance->DungeonInfos;

	uint32 Size = DungeonInfos.EnemyClasses.Num();
	float FullHealth = 0.f, Health = 0.f;
	for (uint32 Idx = 0; Idx < Size; ++Idx)
	{
		FTransform Trans = DungeonInfos.EnemyTransforms[Idx];

		FVector SpawnLoc = Trans.GetLocation();
		FRotator SpawnRot = Trans.Rotator();
		FActorSpawnParameters Param;
		Param.Name = DungeonInfos.EnemyNames[Idx];
		UClass* Class = DungeonInfos.EnemyClasses[Idx];
		AActor* NewActor = world->SpawnActor(
			Class, &SpawnLoc, &SpawnRot, Param
		);
		NewActor->SetActorTransform(Trans);
		NewActor->SetActorHiddenInGame(false);

		FullHealth = DungeonInfos.EnemyFullHealth[Idx];
		Health = DungeonInfos.EnemyHealth[Idx];

		IEnemyInterface::Execute_ActorSaveDataLoaded(NewActor, FullHealth, Health);
	}
}

/////////////////////////////////////////////////////////
// Player

void URoguelikeGameInstance::SetPlayer(ARoguelikeCharacter* InPlayer)
{
	Player = InPlayer;
}

ARoguelikeCharacter* URoguelikeGameInstance::GetPlayer()
{
	if (Player != nullptr) return Player;
	return Player = Cast<ARoguelikeCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

/////////////////////////////////////////////////////////
// Enemy

FVector URoguelikeGameInstance::GetRndEnemyLoiterPos()
{
	uint32 Size = EnemyLoiterPos.Num();
	if (Size == 0) return FVector::ZeroVector;

	uint32 Index = FMath::RandRange(0, Size - 1);
	if (EnemyLoiterPos.IsValidIndex(Index) == true)
	{
		return EnemyLoiterPos[Index];
	}
	return GetRndEnemyLoiterPos();
}

void URoguelikeGameInstance::SpawnEnemy(
	uint8 Num,
	const bool bIsRandom,
	const FVector Pos
)
{
	if (CheckWorld() == nullptr) return;

	int R = 0;
	FVector SpawnPos;
	for (uint32 Idx = 0; Idx < Num; ++Idx)
	{
		R = FMath::RandRange(0, 4);
		if (bIsRandom == true)
		{
			SpawnPos = GetRndEnemyLoiterPos();
		}
		else
		{
			SpawnPos = Pos;
		}
		SpawnPos.Z = 10.f;
		if (R == 0)
		{
			AMage* res = world->SpawnActor<AMage>(
				MageClass, SpawnPos, FRotator::ZeroRotator
				);
		}
		if (R == 1)
		{
			AKing* res = world->SpawnActor<AKing>(
				KingClass, SpawnPos, FRotator::ZeroRotator
				);
		}
		if (R == 2)
		{
			AGrunt* res = world->SpawnActor<AGrunt>(
				GruntClass, SpawnPos, FRotator::ZeroRotator
				);
		}
		if (R == 3)
		{
			ACrossbowman* res = world->SpawnActor<ACrossbowman>(
				CrossbowmanClass, SpawnPos, FRotator::ZeroRotator
				);
		}
		if (R == 4)
		{
			ASwordman* res = world->SpawnActor<ASwordman>(
				SwordmanClass, SpawnPos, FRotator::ZeroRotator
				);
		}
	}
}
