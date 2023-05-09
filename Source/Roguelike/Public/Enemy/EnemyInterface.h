// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Public/Enemy/EnemyAIController.h"
#include "EnemyInterface.generated.h"

class ARoguelikeCharacter;
class USkeletalMeshComponent;
class UAnimInstance;
class UAnimMontage;
class URoguelikeGameInstance;
class UEnemyWidget;
class UPawnSensingComponent;

UENUM()
enum class EHealthType : uint8
{
	Zero = 0,
	Half = 1,
	Full = 2,
	None = 255,
};
ENUM_CLASS_FLAGS(EHealthType)

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()

		UEnemyInterface(const FObjectInitializer& ObjectInitializer);
};

class ROGUELIKE_API IEnemyInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/////////////////////////////////
	// Status
	float GetHealthPercentage() const { return HealthPercentage; }
	float GetHealth() const { return Health; }
	float GetFullHealth() const { return Health; }

	/////////////////////////////////
	// Animation
	void PlayAnimation(UAnimMontage* PlayMontage);
	bool IsPlayingAnim(UAnimMontage* PlayMontage);
	void StopAnimation(UAnimMontage* PlayMontage = nullptr);

	/////////////////////////////////
	// Save
	UFUNCTION(BlueprintNativeEvent)
		void ActorSaveDataLoaded(const float InFullHealth, const float InHealth);

	UFUNCTION(BlueprintNativeEvent)
		void ActorSaveDataSaved(float& OutFullHealth, float& OutHealth);

private:
	UAnimInstance* AnimInstance;
	UEnemyWidget* EnemyUI;

	/////////////////////////////////
	// 
	URoguelikeGameInstance* GetGameInstance();

	/////////////////////////////////
	// Status
	EHealthType UpdateHealthType(const float HealthPercentage);

	/////////////////////////////////
	// Move
	EPathFollowingRequestResult::Type MoveResult;

	TArray<FVector> SafetyMovable = {
		{-750.f, 0.f, 0.f}, {750.f, 0.f, 0.f}, {0.f, -750.f, 0.f}, {0.f, 750.f, 0.f},
		{-500.f, 500.f, 0.f}, {500.f, 500.f, 0.f}, {500.f, -500.f, 0.f}, {-500.f, -500.f, 0.f},
	};

	/////////////////////////////////
	// Math
	FRotator MakeRotFromX(const FVector& X);

protected:
	/////////////////////////////////
	// 
	URoguelikeGameInstance* GameInstance;
	AEnemyAIController* AIController;
	ARoguelikeCharacter* Player;
	USkeletalMeshComponent* MeshComp;
	UPawnSensingComponent* PawnSensingComp;

	/////////////////////////////////
	// Notify
	void ForceNotifyTarget(ACharacter* Parent);
	void ForceLostTarget();

	bool bIsCanSeeUI;
	void ForceCanSeeUI() { bIsCanSeeUI = true; }

	bool CheckIsInNotify(ACharacter* Parent);

	/////////////////////////////////
	// UMG
	void OpenEnemyUI(float InHealthPercentage = 1.f);
	void CloseEnemyUI();
	void UpdateEnemyUIPos(ACharacter* ParentCharacter);

	/////////////////////////////////
	// Status
	float FullHealth;
	float Health;
	float HealthPercentage;
	EHealthType HealthType;

	void UpdateHealth(float ChangeHealth);

	/////////////////////////////////
	// Move
	float DistanceToTarget;
	FVector GetNewLocation(FVector InLocation);
	/*
		Note: You should use closed room game like a dungeon.
		because when not exists wall, the character fall off cliff.
	*/
	EPathFollowingRequestResult::Type MoveToLocation(FVector InLocation);
	EPathFollowingRequestResult::Type MoveToTarget();
	void MoveSafety();

	/////////////////////////////////
	// Math
	/* Rotate to target */
	FRotator FindLookAtRotation(const FVector& Start, const FVector& Target);
};
