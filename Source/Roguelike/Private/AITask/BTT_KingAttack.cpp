// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/AITask/BTT_KingAttack.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "Engine.h"

EBTNodeResult::Type UBTT_KingAttack::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory
)
{
	MyController = OwnerComp.GetAIOwner();
	if (MyController == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Controlelr is null");
		return EBTNodeResult::Failed;
	}

	Parent = Cast<AKing>(MyController->GetCharacter());
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
		if (FMath::RandRange(0, 4) == FMath::RandRange(0, 4))
		{
			Parent->Summon();
			return EBTNodeResult::Succeeded;
		}
		float tmp = FMath::RandRange(0, 1);
		int RndAttack = ((int)tmp + (int)(tmp * 10) % 10) % 2;
		switch (RndAttack)
		{
		case 0:
			Parent->PlayAnimation(Parent->Attack1Montage);
			break;
		default:
			Parent->PlayAnimation(Parent->Attack2Montage);
			break;
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}