// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Attack/Bullet.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"

#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/PrimitiveComponent.h"

#include "Engine.h"

// Sets default values
ABullet::ABullet()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Comp"));
	SetRootComponent(SceneComponent);

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetupAttachment(SceneComponent);
	CollisionComp->SetGenerateOverlapEvents(true);
	//CollisionComp->bMultiBodyOverlap = true;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 1500.0f;

	ParticleSysComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Comp"));
	ParticleSysComponent->SetupAttachment(CollisionComp);

	// Die after 3 seconds by default
	InitialLifeSpan = 3;
	Tags.Add(Tag);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnComponentOverlap);
}

void ABullet::OnComponentOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (OtherActor->ActorHasTag(AttackableActor) == true)
	{
		if (OtherComp->ComponentHasTag("Ignore") == true) return;
		if (OtherActor->ActorHasTag("Wall") == true) { Destroy(); return; }

		APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);

		OtherActor->TakeDamage(BulletDamage, DamageEvent, Controller, this);
		Destroy();
	}
}

void ABullet::SetTarget(APawn* Pawn)
{
	ProjectileMovement->HomingTargetComponent = Pawn->GetRootComponent();
}
