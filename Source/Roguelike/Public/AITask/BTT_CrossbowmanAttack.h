// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Public/Enemy/Crossbowman.h"
#include "TimerManager.h"
#include "BTT_CrossbowmanAttack.generated.h"

class AEnemyAIController;
class ARoguelikeCharacter;
class UBehaviorTreeComponent;

/**
 *
 */
UCLASS()
class ROGUELIKE_API UBTT_CrossbowmanAttack : public UBTTaskNode
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
		ACrossbowman* Parent = nullptr;

	UPROPERTY()
		ACharacter* Target = nullptr;
};
