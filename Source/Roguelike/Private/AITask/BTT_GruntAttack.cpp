// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/AITask/BTT_GruntAttack.h"

#include "BehaviorTree/BehaviorTreeComponent.h"

#include "Engine.h"

EBTNodeResult::Type UBTT_GruntAttack::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory
)
{
	MyController = OwnerComp.GetAIOwner();
	if (MyController == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Controlelr is null");
		return EBTNodeResult::Failed;
	}

	Parent = Cast<AGrunt>(MyController->GetCharacter());
	if (Parent == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Grunt Parent is null");
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
		float tmp = FMath::RandRange(0, 3);
		int RndAttack = ((int)tmp + (int)(tmp * 10) % 10) % 4;
		switch (RndAttack)
		{
		case 0:
			Parent->PlayAnimation(Parent->SlamAttackMontage);
			break;
		case 1:
			Parent->PlayAnimation(Parent->SwingAttackMontage);
			break;
		case 2:
			Parent->PlayAnimation(Parent->SpinAttackMontage);
			break;
		default:
			Parent->PlayAnimation(Parent->NormalAttackMontage);
			break;
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}