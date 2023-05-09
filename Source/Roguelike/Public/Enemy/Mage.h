// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Public/Enemy/EnemyInterface.h"
#include "Mage.generated.h"

class ARoguelikeCharacter;
class UAnimMontage;
class UParticleSystem;
class UCapsuleComponent;

UCLASS()
class ROGUELIKE_API AMage :
	public ACharacter, public IEnemyInterface
{
	GENERATED_BODY()

		UPROPERTY()
		UCapsuleComponent* CapsuleComp = nullptr;
	/////////////////////////////////////////////
	// Enemy Move
	UPROPERTY()
		FVector NewLocation = FVector::ZeroVector;
	void mGetNewLocation();

	FTimerHandle TimerHandler;

	UPROPERTY(EditAnywhere, Category = "Tutorial")
		bool InTutorial = false;

	////////////////////////////////////////////
	// Animation
	UPROPERTY(EditAnywhere)
		UAnimMontage* SpawnMontage = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* DeathMontage = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* HitMontage = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* SummonMontage = nullptr;

	///////////////////////////////////////////
	//
	UFUNCTION()
		void OnComponentOverlap(
			class UPrimitiveComponent* OverlappedComponent,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

	UFUNCTION()
		virtual void EndPlay(
			const EEndPlayReason::Type EndPlayReason
		) override;

	virtual void ActorSaveDataLoaded_Implementation(const float InFullHealth, const float InHealth);
	virtual void ActorSaveDataSaved_Implementation(float& FullHealth, float& Health);

public:
	// Sets default values for this character's properties
	AMage();

	/////////////////////////////////////
	//// Animation
	UPROPERTY(EditAnywhere)
		UAnimMontage* FireAttackMontage = nullptr;
	UPROPERTY()
		float FireDamage = 150.f;

	UPROPERTY(EditAnywhere)
		UAnimMontage* RapidAttackMontage = nullptr;
	UPROPERTY()
		float RapidDamage = 100.f;

	UPROPERTY(EditAnywhere)
		UAnimMontage* ChargeAttackMontage = nullptr;
	UPROPERTY()
		float ChargeDamage = 200.f;

	UPROPERTY(EditAnywhere)
		UAnimMontage* BlastAttackMontage = nullptr;
	UPROPERTY()
		float BlastDamage = 300.f;

	void Summon();
	void mSummon();

	///////////////////////////////////////
	// Particle

	UPROPERTY(EditAnywhere, Category = Particle)
		UParticleSystem* FireParticle = nullptr;

	UPROPERTY(EditAnywhere, Category = Particle)
		UParticleSystem* WaterParticle = nullptr;

	UPROPERTY(EditAnywhere, Category = Particle)
		UParticleSystem* ThunderParticle = nullptr;

	UPROPERTY(EditAnywhere, Category = Particle)
		UParticleSystem* BlastParticle = nullptr;

	///////////////////////////////////////
	//// Status
	UFUNCTION()
		virtual float TakeDamage(
			float Damage,
			struct FDamageEvent const& DamageEvent,
			class AController* EventInstigator,
			class AActor* DamageCauser
		) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
