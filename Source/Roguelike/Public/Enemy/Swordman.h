// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Public/Enemy/EnemyInterface.h"
#include "Swordman.generated.h"

class ARoguelikeCharacter;
class FTimerManager;

UCLASS()
class ROGUELIKE_API ASwordman
	: public ACharacter, public IEnemyInterface
{
	GENERATED_BODY()

		UPROPERTY()
		UCapsuleComponent* CapsuleComp = nullptr;

	UPROPERTY()
		FVector NewLocation = FVector::ZeroVector;
	void mGetNewLocation();

	FTimerHandle TimerHandler;

	UPROPERTY(EditAnywhere, Category = "Tutorial")
		bool InTutorial = false;

	////////////////////////////////
	// Animation
	UPROPERTY(EditAnywhere)
		UAnimMontage* SpawnMontage = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* DeathMontage = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* ShieldBlockMontage = nullptr;

	UPROPERTY()
		uint8 BlockCount = 0;

	UPROPERTY(EditAnywhere)
		UAnimMontage* ShieldBashMontage = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* HitMontage = nullptr;

	////////////////////////////////
	// Status
	UPROPERTY()
		float SmallSwingDamage = 50.f;

	UPROPERTY()
		float BigSwingDamage = 100.f;

	UPROPERTY()
		float FrenzieDamage = 25.f;

	////////////////////////////////
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

	virtual void ActorSaveDataLoaded_Implementation(const float InFullHealth, const float InHealth);
	virtual void ActorSaveDataSaved_Implementation(float& FullHealth, float& Health);

public:
	// Sets default values for this character's properties
	ASwordman();

	////////////////////////////////////
	// Animation
	UPROPERTY(EditAnywhere)
		UAnimMontage* SmallSwingAttackMontage = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* BigSwingAttackMontage = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* FrenzieAttackMontage = nullptr;

	/////////////////////////////////////
	// Status
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

public:
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

};
