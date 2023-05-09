// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/Stair.h"
#include "Public/Components/DungeonPortal.h"

#include "Public/Dungeon/GenerateExterior.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Particles/ParticleSystemComponent.h"

#include "Public/RoguelikeGameInstance.h"

// Sets default values
AStair::AStair()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Comp"));
	SetRootComponent(SceneComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> Find(TEXT("/Engine/BasicShapes/Cube"));
	Iwa = Find.Object;

	Stair1MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stair Comp1"));
	Stair1MeshComp->SetupAttachment(SceneComponent);
	Stair1MeshComp->SetStaticMesh(Iwa);
	Stair1MeshComp->SetRelativeScale3D({ 2.f, 2.f, 0.5f });

	Stair2MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stair Comp2"));
	Stair2MeshComp->SetStaticMesh(Iwa);
	Stair2MeshComp->SetupAttachment(Stair1MeshComp);
	Stair2MeshComp->SetRelativeScale3D({ 0.8f, 1.f, 1.f });
	Stair2MeshComp->SetRelativeLocation({ -10.f , 0.f, 30.f });

	Stair3MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stair Comp3"));
	Stair3MeshComp->SetStaticMesh(Iwa);
	Stair3MeshComp->SetupAttachment(Stair2MeshComp);
	Stair3MeshComp->SetRelativeScale3D({ 0.75f, 1.f, 1.f });
	Stair3MeshComp->SetRelativeLocation({ -12.5f , 0.f, 60.f });

	Stair4MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stair Comp4"));
	Stair4MeshComp->SetStaticMesh(Iwa);
	Stair4MeshComp->SetupAttachment(Stair3MeshComp);
	Stair4MeshComp->SetRelativeScale3D({ 0.6666667f, 1.f, 1.f });
	Stair4MeshComp->SetRelativeLocation({ -16.666666f, 0.f, 90.f });
	Stair4MeshComp->OnComponentHit.AddDynamic(this, &AStair::OnHit);

	ConstructorHelpers::FObjectFinder<UMaterialInterface> FindMat(TEXT("/Game/Pack/Material/M_StoneWall_Inst"));
	Material = FindMat.Object;

	Stair1MeshComp->SetMaterial(0, Material);
	Stair2MeshComp->SetMaterial(0, Material);
	Stair3MeshComp->SetMaterial(0, Material);
	Stair4MeshComp->SetMaterial(0, Material);

	ConstructorHelpers::FObjectFinder<UParticleSystem> FindPart(TEXT("/Game/FXVarietyPack/Particles/P_ky_healAura.P_ky_healAura"));
	Portal = FindPart.Object;

	ParticleSysComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Comp"));
	ParticleSysComponent->SetupAttachment(SceneComponent);
	ParticleSysComponent->SetTemplate(Portal);

	Tags.Add("Portal");
}

void AStair::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	if (!bAllowHit) return;

	if (OtherActor->ActorHasTag("Player") == true)
	{
		if (GetGameInstance() == nullptr) return;

		EPortalType Type = GameInstance->GetStairType();
		if (Type == EPortalType::NextFloor)
		{
			Destroy();

			//GameInstance->AddPlayTimeSeconds(GetWorld()->GetTimeSeconds());
			GameInstance->AddPlayTime(GetWorld()->GetTimeSeconds());
			GameInstance->AddFloorCount();
			GameInstance->SetOpenLevelType(EOpenLevelType::Main);
			GameInstance->BeginLoadingScreen(GameInstance->GetMainLevel());
		}
	}
}


////////////////////////////////////////////////////////////////
//

URoguelikeGameInstance* AStair::GetGameInstance()
{
	return GameInstance = URoguelikeGameInstance::GetGameInstance();
}