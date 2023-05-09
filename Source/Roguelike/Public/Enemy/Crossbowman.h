// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Public/Enemy/EnemyInterface.h"
#include "Crossbowman.generated.h"

class ARoguelikeCharacter;
class UCapsuleComponent;
class UAnimMontage;
class FTimerManager;

UCLASS()
class ROGUELIKE_API ACrossbowman :
	public ACharacter, public IEnemyInterface
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* CapsuleComp = nullptr;

	/////////////////////////////////////////////
	// Enemy Move
	UPROPERTY()
		FVector NewLocation = FVector::ZeroVector;
	void mGetNewLocation();

	FTimerHandle TimerHandler;
	UPROPERTY(EditAnywhere, Category = "Tutorial")
		bool InTutorial = false;
	/////////////////////////////////////////////
	// Animation 
	UPROPERTY(EditAnywhere)
		UAnimMontage* SpawnMontage = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* DeathMontage = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* HitMontage = nullptr;


	/////////////////////////////////////////////
	//
	UFUNCTION(BlueprintCallable)
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

public:
	// Sets default values for this character's properties
	ACrossbowman();

	virtual void ActorSaveDataLoaded_Implementation(const float InFullHealth, const float InHealth);
	virtual void ActorSaveDataSaved_Implementation(float& FullHealth, float& Health);

	/////////////////////////////////////
	//// Animation
	UPROPERTY(EditAnywhere)
		UAnimMontage* NormalAttackMontage = nullptr;
	UPROPERTY()
		float NormalDamage = 250.f;

	UPROPERTY(EditAnywhere)
		UAnimMontage* RapidAttackMontage = nullptr;
	UPROPERTY()
		float RapidDamage = 100.f;

	UPROPERTY(EditAnywhere)
		UAnimMontage* WhackAttackMontage = nullptr;
	UPROPERTY()
		float WhackDamage = 150.f;

	///////////////////////////////////////
	// Particle

	UPROPERTY(EditAnywhere, Category = Particle)
		UParticleSystem* FireParticle = nullptr;

	UPROPERTY(EditAnywhere, Category = Particle)
		UParticleSystem* WaterParticle = nullptr;

	UPROPERTY(EditAnywhere, Category = Particle)
		UParticleSystem* ThunderParticle = nullptr;

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
