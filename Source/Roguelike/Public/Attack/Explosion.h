// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

class USphereComponent;
class UParticleSystem;
class UParticleSystemComponent;

UCLASS()
class ROGUELIKE_API AExplosion : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExplosion();

	UPROPERTY()
		FName AttackAbleActorTag = "";

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = ptoAttack)
		float ExplosionDamage = 150.0f;

	UPROPERTY(VisibleAnywhere, Category = ptoAttack)
		USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = ptoAttack)
		USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, Category = ptoAttack)
		UParticleSystemComponent* ParticleSysComponent;

	UPROPERTY(EditAnywhere, Category = ptoAttack)
		UParticleSystem* ExplosionParticle;

	UFUNCTION()
		void OnComponentOverlap(
			class UPrimitiveComponent* OverlappedComponent,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);
};
