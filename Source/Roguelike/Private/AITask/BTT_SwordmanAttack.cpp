// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/AITask/BTT_SwordmanAttack.h"

#include "BehaviorTree/BehaviorTreeComponent.h"

#include "Engine.h"

EBTNodeResult::Type UBTT_SwordmanAttack::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory
)
{
	MyController = OwnerComp.GetAIOwner();
	if (MyController == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Controlelr is null");
		return EBTNodeResult::Failed;
	}

	Parent = Cast<ASwordman>(MyController->GetCharacter());
	if (Parent == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Swordman Parent is null");
		return EBTNodeResult::Failed;
	}

	AIController = Cast<AEnemyAIController>(MyController);
	if (AIController == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "AIControlelr is null");
		return EBTNodeResult::Failed;
	}

	Target = AIController->GetPlayerKey();
	if (Target == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Target is null");
		return EBTNodeResult::Failed;
	}

	float Distance = Parent->GetDistanceTo(Target);
	if (Distance < 300)
	{
		float tmp = FMath::RandRange(0, 2);
		int RndAttack = ((int)tmp + (int)(tmp * 10) % 10) % 3;
		switch (RndAttack)
		{
		case 0:
			Parent->PlayAnimation(Parent->FrenzieAttackMontage);
			break;
		case 1:
			Parent->PlayAnimation(Parent->BigSwingAttackMontage);
			break;
		default:
			Parent->PlayAnimation(Parent->SmallSwingAttackMontage);
			break;
		}
		return EBTNodeResult::Succeeded;

	}

	return EBTNodeResult::Failed;
}