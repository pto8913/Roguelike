// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Public/Enemy/EnemyInterface.h"
#include "King.generated.h"

class ARoguelikeCharacter;
class USkeletalMeshComponent;
class UCapsuleComponent;
class UEnemyWidget;
class FTimerManager;

UCLASS()
class ROGUELIKE_API AKing
	: public ACharacter, public IEnemyInterface
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
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
	UPROPERTY()
		UAnimInstance* AnimInstance = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* SpawnMontage = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* DeathMontage = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* SummonMontage = nullptr;

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
	AKing();

	virtual void ActorSaveDataLoaded_Implementation(const float InFullHealth, const float InHealth);
	virtual void ActorSaveDataSaved_Implementation(float& FullHealth, float& Health);

	/////////////////////////////////////
	//// Animation
	UPROPERTY(EditAnywhere)
		UAnimMontage* Attack1Montage = nullptr;
	UPROPERTY()
		float Attack1Damage = 150.f;

	UPROPERTY(EditAnywhere)
		UAnimMontage* Attack2Montage = nullptr;
	UPROPERTY()
		float Attack2Damage = 200.f;

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

	/*UPROPERTY()
		EHealthType HealthType = EHealthType::Full;*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
