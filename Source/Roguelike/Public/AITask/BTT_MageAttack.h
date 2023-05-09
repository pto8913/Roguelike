// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Public/Enemy/Mage.h"
#include "BTT_MageAttack.generated.h"

class AMage;

class UBehaviorTreeComponent;
/**
 *
 */
UCLASS()
class ROGUELIKE_API UBTT_MageAttack : public UBTTaskNode
{
	GENERATED_BODY()

		virtual EBTNodeResult::Type ExecuteTask(
			UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory
		) override;

	UPROPERTY()
		AAIController* MyController = nullptr;

	UPROPERTY()
		AMage* Parent = nullptr;

	UPROPERTY()
		AEnemyAIController* AIController = nullptr;

	UPROPERTY()
		ACharacter* Target = nullptr;
};
