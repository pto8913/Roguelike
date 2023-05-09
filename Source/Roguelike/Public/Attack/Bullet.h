// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Bullet.generated.h"

class USphereComponent;
class USceneComponent;

UCLASS()
class ROGUELIKE_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/* Set Homing Target */
	void SetTarget(APawn* Pawn);

	/* Need Positive value */
	float BulletDamage = 100;

	/* Player / Enemy */
	FName ParentActor;
	/* Player / Enemy */
	FName AttackableActor;
	/* Player / Enemy Attack */
	FName Tag;

	UPROPERTY(EditAnywhere)
		float LifeSpan = 3.f;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* SceneComponent;

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly)
		USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* ParticleSysComponent;

	UFUNCTION(BlueprintCallable)
		void OnComponentOverlap(
			class UPrimitiveComponent* OverlappedComponent,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
