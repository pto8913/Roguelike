// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Public/Enemy/King.h"
#include "BTT_KingAttack.generated.h"

class AEnemyAIController;
class ARoguelikeCharacter;
class UBehaviorTreeComponent;

/**
 *
 */
UCLASS()
class ROGUELIKE_API UBTT_KingAttack : public UBTTaskNode
{
	GENERATED_BODY()

		virtual EBTNodeResult::Type ExecuteTask(
			UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory
		) override;

	UPROPERTY()
		AAIController* MyController = nullptr;

	UPROPERTY()
		AKing* Parent = nullptr;

	UPROPERTY()
		AEnemyAIController* AIController = nullptr;

	UPROPERTY()
		ACharacter* Target = nullptr;
};
