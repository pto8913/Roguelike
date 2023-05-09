// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Enemy/EnemyAIController.h"
#include "Public/Player/RoguelikeCharacter.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine.h"


AEnemyAIController::AEnemyAIController(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	PlayerActorKeyName = "PlayerActor";

	ConstructorHelpers::FObjectFinder<UBehaviorTree> Finder(TEXT("/Game/EnemyAI/BT_EnemyAI.BT_EnemyAI"));
	BehaviorTree = Finder.Object;
}


void AEnemyAIController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BlackboardComp != nullptr && BehaviorTree != nullptr)
	{
		BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

		if (BehaviorComp != nullptr)
		{
			BehaviorComp->StartTree(*BehaviorTree);
		}
	}
}


void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();

	BehaviorComp->StopTree();
}


void AEnemyAIController::SetPlayerKey(ARoguelikeCharacter* InPlayer)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(PlayerActorKeyName, InPlayer);
	}
}


ACharacter* AEnemyAIController::GetPlayerKey()
{
	if (BlackboardComp == nullptr) return nullptr;
	return Cast<ACharacter>(BlackboardComp->GetValueAsObject(PlayerActorKeyName));
}
