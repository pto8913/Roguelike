// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Public/Enemy/EnemyInterface.h"
#include "RoguelikeCharacter.generated.h"

class ARoguelikeGameMode;
class AMazeGameMode;
class URoguelikeGameInstance;
class UOptionWidget;
class UPlayerWidget;
class UResultWidget;
class UMapWidget;
class UAnimInstance;
class UAnimMontage;
class USkeletalMeshComponent;
class UParticleSystem;
class UParticleSystemComponent;
class USphereComponent;
class UBoxComponent;

UENUM()
enum class EPlayerState : uint8
{
	Idle = 0,
	Attack = 1,
	None = 255,
};
ENUM_CLASS_FLAGS(EPlayerState)

UCLASS(config = Game)
class ARoguelikeCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY()
		ARoguelikeGameMode* RoguelikeGameMode = nullptr;
	UPROPERTY()
		AMazeGameMode* MazeGameMode = nullptr;

	UPROPERTY()
		URoguelikeGameInstance* GameInstance = nullptr;
	URoguelikeGameInstance* GetGameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = pto, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* MeshComp = nullptr;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = pto, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* Ability_E_Boom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = pto, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* Ability_R_Area = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = pto, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* EnemyNotifyArea = nullptr;
	void EnemyNotifyArea_Deactive();
	void EnemyNotifyArea_Activate();

	//////////////////////////////////////////
	// Widgets 
	UPROPERTY()
		UOptionWidget* OptionUI = nullptr;

	UPROPERTY()
		UMapWidget* MapUI = nullptr;

	UPROPERTY()
		UPlayerWidget* PlayerUI = nullptr;

	///////////////////////////////////////////
	// Animations
	UPROPERTY()
		UAnimInstance* AnimInstance = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* SpawnMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* DeathMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* HitMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* PrimaryAMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* PrimaryBMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* PrimaryCMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* Ability_EMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* Ability_QMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* Ability_RMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* Idle = nullptr;
	void PlayIdle();

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* Ability_F_FinishMontage = nullptr;
	///////////////////////////////////////////
	// Atatck
	UPROPERTY()
		int ComboAttack = 0;

	UPROPERTY()
		bool bComboAttackSave = true;

	void Attack();
	void Ability_Q();
	UPROPERTY()
		float Ability_QDamage = 200.f;

	void Ability_E();
	UPROPERTY()
		float Ability_EDamage = 200.f;

	void Ability_R();
	UPROPERTY()
		float Ability_RDamage = 200.f;

	void Ability_F();
	UPROPERTY()
		float Ability_FAddtive = 1.3f;
	UPROPERTY()
		float Ability_FDamage = 100.f;
	///////////////////////////////////////////
	// Particle
	UPROPERTY(EditAnywhere, Category = "Attack")
		UParticleSystem* AttackHitParticle;

	///////////////////////////////////////////
	// Status 
	UPROPERTY(EditAnywhere)
		float FullHealth = 5000.f;

	UPROPERTY()
		float Health = FullHealth;

	UPROPERTY()
		float HealthPercentage = 1.f;

	EHealthType UpdateHealthType(const float HealthPercentage);
	UPROPERTY()
		EHealthType HealthType;

	void UpdateHealth(float ChangeHealth);

	EPlayerState CurrentState = EPlayerState::Idle;

	///////////////////////////////////////////
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

public:

	ARoguelikeCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USphereComponent* Ability_E_TeleportTarget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USphereComponent* Ability_F_ExplosionTarget = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attack")
		UParticleSystem* Ability_F_Explosion = nullptr;

	UFUNCTION()
		virtual float TakeDamage(
			float Damage,
			struct FDamageEvent const& DamageEvent,
			class AController* EventInstigator,
			class AActor* DamageCauser
		) override;

	///////////////////////////////////////////
	// Camera

	void SetCameraArmLength(const float In) { CameraBoom->TargetArmLength = In; }
	float GetCameraArmLength() const { return CameraBoom->TargetArmLength; }

	///////////////////////////////////////////
	// Status
	float GetPlayerHealthPercentage() const { return HealthPercentage; }
	EPlayerState GetPlayerState() const { return CurrentState; }
	void SetPlayerState(EPlayerState In) { CurrentState = In; }

	///////////////////////////////////////////
	// Animations
	float PlayAnimation(UAnimMontage* PlayMontage);
	bool IsPlaying(UAnimMontage* PlayMontage);
	void SetComboAttackSave(const bool InSave) { bComboAttackSave = InSave; }
	bool GetComboAttackSave() { return bComboAttackSave; }

	UPROPERTY(EditAnywhere, Category = "Attack")
		UAnimMontage* Ability_FMontage = nullptr;
	void Ability_F_AddDamage() { Ability_FDamage *= Ability_FAddtive; }
	void Ability_F_Finish();
	void Ability_F_Charge(float In);
	float GetAbility_FDamage() const { return Ability_FDamage; }

	void Ability_E_Implementation();

	///////////////////////////////////////////
	// Attack

	UPROPERTY()
		float NormalDamage = 100.f;

	/* UltimateExplode */
	UPROPERTY()
		float ExplosionDamage = 300.f;

	void Ability_R_Activate();
	void Ability_R_Deactivate();


protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void OpenOption();

	void OpenMap();
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

