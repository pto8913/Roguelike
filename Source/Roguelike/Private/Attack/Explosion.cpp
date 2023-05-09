// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Attack/Explosion.h"

#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/PrimitiveComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"

#include "Engine.h"

// Sets default values
AExplosion::AExplosion()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Comp"));
	SceneComponent->SetRelativeScale3D(FVector(9.0f, 9.0f, 9.0f));
	SetRootComponent(SceneComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision Comp"));
	SphereCollision->SetupAttachment(SceneComponent);
	SphereCollision->SetRelativeScale3D(FVector(9.0f, 9.0f, 9.0f));
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_OverlapAll_Deprecated, ECollisionResponse::ECR_Overlap);

	ParticleSysComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Comp"));
	ParticleSysComponent->SetupAttachment(SceneComponent);

	Tags.Add("Explosion");
	InitialLifeSpan = 0.3f;
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AExplosion::OnComponentOverlap);
}

void AExplosion::OnComponentOverlap(
	class UPrimitiveComponent* OverlappedComponent,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (OtherActor->ActorHasTag(AttackAbleActorTag) == true)
	{
		if (OtherComp->ComponentHasTag("Ignore") == true) return;

		APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		OtherActor->TakeDamage(ExplosionDamage, DamageEvent, Controller, this);
	}
}