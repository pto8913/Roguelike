// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "EnemyAIController.generated.h"

class ARoguelikeCharacter;

UCLASS()
class ROGUELIKE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
private:
	AEnemyAIController(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY()
		UBehaviorTreeComponent* BehaviorComp = nullptr;

	UPROPERTY()
		UBlackboardComponent* BlackboardComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = ptoAI)
		FName PlayerActorKeyName = "";

public:
	UPROPERTY(EditAnywhere)
		bool bIsFindPlayer = false;

	void SetPlayerKey(ARoguelikeCharacter* InPlayer);
	ACharacter* GetPlayerKey();

	virtual void OnPossess(class APawn* InPawn) override;

	virtual void OnUnPossess() override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

};
