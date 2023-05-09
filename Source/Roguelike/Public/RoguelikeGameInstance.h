// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Public/Enemy/Mage.h"
#include "Public/Enemy/King.h"
#include "Public/Enemy/Grunt.h"
#include "Public/Enemy/Crossbowman.h"
#include "Public/Enemy/Swordman.h"

#include "UMG/TitleWidget.h"
#include "UMG/NotifierWidget.h"
#include "UMG/OptionWidget.h"
#include "UMG/MapWidget.h"
#include "UMG/PlayerWidget.h"
#include "UMG/EnemyWidget.h"
#include "UMG/LoadingWidget.h"
#include "UMG/SelectDataWidget.h"
#include "UMG/ResultWidget.h"
#include "UMG/ScoreWidget.h"
#include "Tutorial/TutorialWidget.h"

#include "Public/Templates/ptoEnums.h"
#include "Public/Player/RoguelikeCharacter.h"

#include "RoguelikeGameInstance.generated.h"

UCLASS()
class ROGUELIKE_API URoguelikeGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY()
		UWorld* world = nullptr;
	UWorld* CheckWorld();

	UPROPERTY()
		ARoguelikeCharacter* Player = nullptr;

	UPROPERTY()
		APlayerController* Controller = nullptr;

	/* for level */
	EOpenLevelType OpenLevelType = EOpenLevelType::None;
	
	/* for loading game name */
	UPROPERTY()
		FString LoadSlotName = "";

	/* for loading game index */
	UPROPERTY()
		uint16 LoadSlotIndex = 0;

	//////////////////////////////////
	// UMG

	UPROPERTY()
		UTitleWidget* TitleUI = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UTitleWidget> TitleClass = nullptr;

	UPROPERTY()
		UNotifierWidget* NotifierUI = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UNotifierWidget> NotifierClass = nullptr;

	UPROPERTY()
		UOptionWidget* OptionUI = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UOptionWidget> OptionClass = nullptr;

	UPROPERTY()
		UMapWidget* MapUI = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UMapWidget> MapClass = nullptr;

	UPROPERTY()
		UPlayerWidget* PlayerUI = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UPlayerWidget> PlayerClass = nullptr;

	UPROPERTY()
		UEnemyWidget* EnemyUI = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UEnemyWidget> EnemyClass = nullptr;

	UPROPERTY()
		ULoadingWidget* LoadUI = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ULoadingWidget> LoadClass = nullptr;

	UPROPERTY()
		USelectDataWidget* SelectDataUI = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<USelectDataWidget> SelectDataClass = nullptr;

	UPROPERTY()
		UResultWidget* ResultUI = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UResultWidget> ResultClass = nullptr;
	
	UPROPERTY()
		UUserWidget* SaveCompleteUI = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> SaveCompleteClass = nullptr;;

	UPROPERTY()
		UUserWidget* SaveFailedUI = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> SaveFailedClass = nullptr;

	UPROPERTY()
		UScoreWidget* ScoreUI = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UScoreWidget> ScoreClass = nullptr;

	UPROPERTY()
		UTutorialWidget* TutorialUI = nullptr;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UTutorialWidget> TutorialClass = nullptr;

	////////////////////////////////////
	// Level
	UPROPERTY()
		FString TitleLevel = "/Game/Maps/TitleLevel";
	UPROPERTY()
		FString MainLevel = "/Game/Maps/Main";
	UPROPERTY()
		FString MazeLevel = "/Game/Maps/Maze";
	UPROPERTY()
		FString ScoreLevel = "/Game/Maps/Score";
	UPROPERTY()
		FString TutorialLevel = "/Game/Maps/Tutorial";

	//////////////////////////////////
	// Dungeon
	UPROPERTY()
		uint16 SizeMastering = 0;

	//////////////////////////////////
	//
	UPROPERTY()
		TArray<FVector> EnemyLoiterPos = {};

	UPROPERTY()
		TArray<FScores> Score;

	UPROPERTY()
		FOptions Options;

	UPROPERTY()
		FDungeonInfos DungeonInfos;

public:
	static URoguelikeGameInstance* GetGameInstance();

	UFUNCTION()
		virtual void Init();
	UFUNCTION()
		virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
		virtual void EndLoadingScreen(UWorld* InLoadedWorld);

	void SetScore(float InPlayTime)
	{
		Score.Add(
			FScores(DungeonInfos.DefeatEnemyCount, DungeonInfos.FloorCount, InPlayTime)
		);
	}
	void SetScore(TArray<FScores> In) { Score = In; }
	TArray<FScores> GetScore() const { return Score; }

	void SetOptions(FOptions In) { Options = In; }
	FOptions GetOptions() const { return Options; }

	void SetDungeonInfos(FDungeonInfos In) { DungeonInfos = In; }
	FDungeonInfos GetDungeonInfos() const { return DungeonInfos; }

	///////////////////////////////
	// Level
	FString GetTitleLevel() const { return TitleLevel; }
	FString GetMainLevel() const { return MainLevel; }
	FString GetMazeLevel() const { return MazeLevel; }
	FString GetScoreLevel() const { return ScoreLevel; }
	FString GetTutorialLevel() const { return TutorialLevel; }

	////////////////////////////////
	// Type

	/* When selecting data, the user can know where is */
	void SetActiveMode(EActiveMode In) { DungeonInfos.ActiveMode = In; }
	EActiveMode GetActiveMode() const { return DungeonInfos.ActiveMode; }

	void SetOpenLevelType(EOpenLevelType InType) { OpenLevelType = InType; }
	EOpenLevelType GetOpenLevelType() const { return OpenLevelType; }

	APlayerController* GetPlayerController();

	/* When player goes to from maze to dungeon or from dungeon to maze use this */
	void SetPortalType(EPortalType InType) { DungeonInfos.PortalType = InType; }
	EPortalType GetPortalType() const { return DungeonInfos.PortalType; }

	/*  */
	void SetStairType(EPortalType InType) { DungeonInfos.StairType = InType; }
	EPortalType GetStairType() const { return DungeonInfos.StairType; }

	void AddDefeatEnemyCount() { ++DungeonInfos.DefeatEnemyCount; }
	uint32 GetDefeatEnemyCount() const { return DungeonInfos.DefeatEnemyCount; }

	////////////////////////////////
	// Generator
	void SetIsSpawnPortal(bool bIsSpawn) { DungeonInfos.bIsSpawnPortal = bIsSpawn; }
	bool GetIsSpawnPortal() const { return DungeonInfos.bIsSpawnPortal; }

	void SetPortalLocation(const FVector In) { DungeonInfos.PortalLocation = In; }
	FVector GetPortalLocation()  const { return DungeonInfos.PortalLocation; }

	void SetMasterSize(uint16 In) { DungeonInfos.SizeMaster = In; SizeMastering = In / 100; }
	uint16 GetSizeMaster()  const { return DungeonInfos.SizeMaster; }

	////////////////////////////////
	// Dungeon
	void SetDungeon(TArray<ID> InDungeon) { DungeonInfos.Dungeon = InDungeon; }
	TArray<ID> GetDungeon() const { return DungeonInfos.Dungeon; }

	void SetDungeonSize(uint16 W, uint16 H) { DungeonInfos.DungeonWidth = W; DungeonInfos.DungeonHeight = H; }
	void GetDungeonSize(uint16& W, uint16& H) const { W = DungeonInfos.DungeonWidth; H = DungeonInfos.DungeonHeight; }

	////////////////////////////////
	// Maze
	void SetMaze(TArray<ID> InMaze) { DungeonInfos.Maze = InMaze; }
	TArray<ID> GetMaze() const { return DungeonInfos.Maze; }

	void SetMazeSize(uint16 W, uint16 H) { DungeonInfos.MazeWidth = W; DungeonInfos.MazeHeight = H; }
	void GetMazeSize(uint16& W, uint16& H) const { W = DungeonInfos.MazeWidth; H = DungeonInfos.MazeHeight; }

	////////////////////////////////
	// Gameplay
	void GamePause(const FInputModeDataBase& InData);
	void GameUnPause(const FInputModeDataBase& InData);
	void SetInputMode(const FInputModeDataBase& InData);
	void SetMouseCursor(bool In);

	float GetPlayTime() const { return DungeonInfos.PlayTime; }
	void SetPlayTime(const float In) { DungeonInfos.PlayTime = In; }
	void AddPlayTime(const float In) { DungeonInfos.PlayTime += In; }
	/////////////////////////////////
	// For save

	/*
	@ SlotName "Slot1 ~ Slot3" -> for player
	  SlotName "tmp" -> temporary file for move "Maze to Main" or "Main to Maze", index is 4
	  SlotName "Score" -> for score. index is 5
	  SlotName "Option# -> for options. index is 6
	@ bIsWidget. if true open success UI. false nothing
	@ bIsScore. for use score.
	@ bIsSettings. for use player Options
	*/
	void SaveGame(
		const FString SlotName, const uint16 Index, const bool bIsWidget = true,
		const bool bIsScore = false, const bool bIsOptions = false
	);
	void LoadGame(
		const FString SlotName, const uint16 Index, 
		const bool bIsScore = false, const bool bIsOptions = false
	);

	void SetSlotName(const FString InName) { LoadSlotName = InName; }
	FString GetSlotName() const { return LoadSlotName; }

	void SetSlotIndex(const uint16 Index) { LoadSlotIndex = Index; }
	uint16 GetSlotIndex() const { return LoadSlotIndex; }

	/* To reach floor count */
	void AddFloorCount() { ++DungeonInfos.FloorCount; }
	uint32 GetFloorCount() const { return DungeonInfos.FloorCount; }
	void SetFloorCount(uint32 In) { DungeonInfos.FloorCount = In; }

	/////////////////////////////////
	// Player
	void SetPlayer(ARoguelikeCharacter* InPlayer);
	ARoguelikeCharacter* GetPlayer();

	void SetPlayerFullHealth(float In) { DungeonInfos.PlayerFullHealth = In; }
	float GetPlayerFullHealth() const { return DungeonInfos.PlayerFullHealth; }

	void SetPlayerHealth(float In) { DungeonInfos.PlayerHealth = In; }
	float GetPlayerHealth() const { return DungeonInfos.PlayerHealth; }

	void SetDungeonPlayerEndLocation(FVector In) { DungeonInfos.DungeonPlayerEndLocation = In; }
	FVector GetDungeonPlayerEndLocation() const { return DungeonInfos.DungeonPlayerEndLocation; }

	void SetMazePlayerEndLocation(FVector In) { DungeonInfos.MazePlayerEndLocation = In; }
	FVector GetMazePlayerEndLocation() const { return DungeonInfos.MazePlayerEndLocation; }

	void SetDungeonPlayerStartLocation(FVector In) { DungeonInfos.DungeonPlayerStartLocation = In; }
	FVector GetDungeonPlayerStartLocation() const { return DungeonInfos.DungeonPlayerStartLocation; }

	void SetMazePlayerStartLocation(FVector In) { DungeonInfos.MazePlayerStartLocation = In; }
	FVector GetMazePlayerStartLocation() const { return DungeonInfos.MazePlayerStartLocation; }

	/////////////////////////////////
	// Enemy
	void SetEnemyLoiterPos(const TArray<FVector> InStartCandidate)
	{
		EnemyLoiterPos = InStartCandidate;
	}

	FVector GetRndEnemyLoiterPos();

	void SpawnEnemy(
		const uint8 Num = 11,
		const bool bIsRandom = true,
		const FVector Pos = FVector::ZeroVector
	);

	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> MageClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> KingClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> GruntClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> CrossbowmanClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> SwordmanClass;


	/////////////////////////////////
	// UMG
	UTitleWidget* GetTitleUI() const { return TitleUI; }

	UNotifierWidget* GetNotifierUI() const { return NotifierUI; }

	UOptionWidget* GetOptionUI() const { return OptionUI; }

	UMapWidget* GetMapUI() const { return MapUI; }

	UPlayerWidget* GetPlayerUI() const { return PlayerUI; }
	
	/* Because each enemy needs EnemyUI, create */
	UEnemyWidget* CreateEnemyUI();

	ULoadingWidget* GetLoadUI() const { return LoadUI; }

	USelectDataWidget* GetSelectDataUI() const { return SelectDataUI; }

	UResultWidget* GetResultUI() const { return ResultUI; }

	UScoreWidget* GetScoreUI() const { return ScoreUI; }

	UUserWidget* GetSaveCompleteUI() const { return SaveCompleteUI; }

	UUserWidget* GetSaveFailedUI() const { return SaveFailedUI; }
	
	UTutorialWidget* GetTutorialUI() const { return TutorialUI; }
};
