// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/AITask/BTT_CrossbowmanAttack.h"
#include "Public/Enemy/EnemyAIController.h"

#include "Public/Player/RoguelikeCharacter.h"

#include "BehaviorTree/BehaviorTreeComponent.h"

#include "Engine.h"

EBTNodeResult::Type UBTT_CrossbowmanAttack::ExecuteTask(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory
)
{
    MyController = OwnerComp.GetAIOwner();
    if (MyController == nullptr)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Controlelr is null");
        return EBTNodeResult::Failed;
    }

    AIController = Cast<AEnemyAIController>(MyController);
    if (AIController == nullptr)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "AIControlelr is null");
        return EBTNodeResult::Failed;
    }

    Parent = Cast<ACrossbowman>(MyController->GetCharacter());
    if (Parent == nullptr)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Grunt Parent is null");
        return EBTNodeResult::Failed;
    }

    Target = AIController->GetPlayerKey();
    if (Target == nullptr)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Target is null");
        return EBTNodeResult::Failed;
    }

    float Distance = Parent->GetDistanceTo(Target);

    float tmp = FMath::RandRange(0, 2);
    int RndAttack = ((int)tmp + (int)(tmp * 10) % 10) % 3;
    switch (RndAttack)
    {
    case 0:
        if (Distance < 150)
        {
            Parent->PlayAnimation(Parent->WhackAttackMontage);
            break;
        }
    case 1:
        Parent->PlayAnimation(Parent->RapidAttackMontage);
        break;
    default:
        Parent->PlayAnimation(Parent->NormalAttackMontage);
        break;
    }
    return EBTNodeResult::Succeeded;
}