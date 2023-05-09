// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Public/Enemy/EnemyInterface.h"
#include "Grunt.generated.h"

class ARoguelikeCharacter;
class UAnimMontage;
class UEnemyWidget;
class UCapsuleComponent;
class URoguelikeGameInstance;
class FTimerManager;

UCLASS()
class ROGUELIKE_API AGrunt
	: public ACharacter, public IEnemyInterface
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* CapsuleComp = nullptr;

	/////////////////////////////////////////////
	// Enemy Move
	FTimerHandle TimerHandler;
	void mGetNewLocation();
	UPROPERTY()
		FVector NewLocation = FVector::ZeroVector;
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
	// Attack Damage
	UPROPERTY()
		float NormalDamage = 100.f;

	UPROPERTY()
		float SwingDamage = 200.f;

	UPROPERTY()
		float SpinDamage = 200.f;

	UPROPERTY()
		float SlamDamage = 300.f;

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
	AGrunt();

	virtual void ActorSaveDataLoaded_Implementation(const float InFullHealth, const float InHealth);
	virtual void ActorSaveDataSaved_Implementation(float& FullHealth, float& Health);

	///////////////////////////////////
	// Animation
	UPROPERTY(EditAnywhere)
		UAnimMontage* NormalAttackMontage = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* SwingAttackMontage = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* SpinAttackMontage = nullptr;

	UPROPERTY(EditAnywhere)
		UAnimMontage* SlamAttackMontage = nullptr;

	/////////////////////////////////////
	// Status
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

public:
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
};
