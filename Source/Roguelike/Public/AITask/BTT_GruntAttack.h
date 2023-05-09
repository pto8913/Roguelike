// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Public/Enemy/Grunt.h"
#include "BTT_GruntAttack.generated.h"

class AEnemyAIController;
class ARoguelikeCharacter;
class UBehaviorTreeComponent;

/**
 *
 */
UCLASS()
class ROGUELIKE_API UBTT_GruntAttack : public UBTTaskNode
{
	GENERATED_BODY()

		virtual EBTNodeResult::Type ExecuteTask(
			UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory
		) override;

	UPROPERTY()
		AAIController* MyController = nullptr;

	UPROPERTY()
		AEnemyAIController* AIController = nullptr;

	UPROPERTY()
		AGrunt* Parent = nullptr;

	UPROPERTY()
		ACharacter* Target = nullptr;
};
