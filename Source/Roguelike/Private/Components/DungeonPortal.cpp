// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/DungeonPortal.h" 

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/SceneComponent.h"

#include "Public/RoguelikeGameInstance.h"
#include "Public/TutorialGameMode.h"

#include "Engine.h"

// Sets default values
ADungeonPortal::ADungeonPortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Comp"));
	SetRootComponent(SceneComponent);

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Comp"));
	CapsuleCollision->SetupAttachment(SceneComponent);
	CapsuleCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_OverlapAll_Deprecated, ECollisionResponse::ECR_Overlap);
	CapsuleCollision->SetGenerateOverlapEvents(true);
	CapsuleCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleFinder(TEXT("/Game/FXVarietyPack/Particles/P_ky_healAura"));
	GateParticle = ParticleFinder.Object;

	ParticleSysComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Comp"));
	ParticleSysComponent->SetupAttachment(SceneComponent);

	Tags.Add("Portal");
}

// Called when the game starts or when spawned
void ADungeonPortal::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ADungeonPortal::OnBeginOverlap);
}


void ADungeonPortal::OnBeginOverlap(
	class AActor* OverlapActor,
	class AActor* OtherActor
)
{
	if (!bAllowOverlap) return;

	if (OtherActor->ActorHasTag("Player") == true)
	{
		if (GetGameInstance() == nullptr) return;

		if (ATutorialGameMode::GetGameMode() != nullptr)
		{
			GameInstance->BeginLoadingScreen(GameInstance->GetTitleLevel());
			return;
		}

		EPortalType Type = GameInstance->GetPortalType();
		if (Type == EPortalType::IntoMaze)
		{
			UE_LOG(LogTemp, Log, TEXT("IntoMaze"));
			Destroy();

			GameInstance->SetIsSpawnPortal(false);
			// Save player location, when player into maze.
			FVector CurrentLocation = GameInstance->GetPlayer()->GetActorLocation();
			GameInstance->SetDungeonPlayerStartLocation(CurrentLocation);

			GameInstance->SetOpenLevelType(EOpenLevelType::Maze);
			GameInstance->AddPlayTime(GetWorld()->GetTimeSeconds());

			//GameInstance->AddPlayTimeSeconds();
			GameInstance->SaveGame("tmp", 4, false);
			GameInstance->BeginLoadingScreen(GameInstance->GetMazeLevel());
		}
		else if (Type == EPortalType::GetOutMaze)
		{
			UE_LOG(LogTemp, Log, TEXT("GetOutMaze"));
			Destroy();

			// Back dungeon start location.
			FVector BackDungeonLocation = GameInstance->GetDungeonPlayerStartLocation();
			GameInstance->GetPlayer()->SetActorLocation(BackDungeonLocation);

			//GameInstance->AddPlayTimeSeconds(GetWorld()->GetTimeSeconds());
			GameInstance->AddPlayTime(GetWorld()->GetTimeSeconds());
			GameInstance->SetOpenLevelType(EOpenLevelType::Load);
			GameInstance->SetSlotName("tmp");
			GameInstance->SetSlotIndex(4);
			GameInstance->BeginLoadingScreen(GameInstance->GetMainLevel());
		}
	}
}


////////////////////////////////////////////////////////////////
//

URoguelikeGameInstance* ADungeonPortal::GetGameInstance()
{
	return GameInstance = URoguelikeGameInstance::GetGameInstance();
}