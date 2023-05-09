// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Enemy/EnemyInterface.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

#include "Components/SkeletalMeshComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "Public/UMG/EnemyWidget.h"
#include "Public/RoguelikeGameInstance.h"
#include "Engine.h"

// Add default functionality here for any IEnemyInterface functions that are not pure virtual.

UEnemyInterface::UEnemyInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

/////////////////////////////////////////////
// Notify

void IEnemyInterface::ForceNotifyTarget(ACharacter* Parent)
{
	if (AIController != nullptr)
	{
		if (bIsCanSeeUI == true)
		{
			if (Player == nullptr)
			{
				if (GameInstance != nullptr)
				{
					Player = GameInstance->GetPlayer();
				}
			}
			if (Player == nullptr)
			{
				return;
			}
			AIController->SetPlayerKey(Player);
			PawnSensingComp->SetPeripheralVisionAngle(360.f);
			PawnSensingComp->SightRadius = Parent->GetHorizontalDistanceTo(Player) * 2;
		}
	}
}

void IEnemyInterface::ForceLostTarget()
{
	if (AIController != nullptr)
	{
		AIController->SetPlayerKey(nullptr);
	}
}

bool IEnemyInterface::CheckIsInNotify(ACharacter* Parent)
{
	if (Player != nullptr)
	{
		FVector CurrentLocation = Parent->GetActorLocation();
		FVector TargetLocation = Player->GetActorLocation();
		float X = CurrentLocation.X;
		float Y = CurrentLocation.Y;
		if ((TargetLocation.X - 1500 <= X && X <= TargetLocation.X + 1500) && (TargetLocation.Y - 1500 <= Y && Y <= TargetLocation.Y + 1500))
		{
			bIsCanSeeUI = true;
			ForceNotifyTarget(Parent);
			return true;
		}
		else
		{
			ForceLostTarget();
			PawnSensingComp->SightRadius = 100.f;
			bIsCanSeeUI = false;
		}
	}
	return false;
}

/////////////////////////////////////////////
// UMG

void IEnemyInterface::OpenEnemyUI(float InHealthPercentage)
{
	if (GetGameInstance() != nullptr)
	{
		EnemyUI = GameInstance->CreateEnemyUI();
		if (EnemyUI != nullptr)
		{
			EnemyUI->OpenEnemyUI(InHealthPercentage);
		}
	}
}

void IEnemyInterface::CloseEnemyUI()
{
	if (EnemyUI != nullptr)
	{
		if (EnemyUI->IsInViewport() == true)
		{
			EnemyUI->CloseEnemyUI();
			GameInstance->AddDefeatEnemyCount();
		}
	}
}

void IEnemyInterface::UpdateEnemyUIPos(ACharacter* ParentCharacter)
{
	if (EnemyUI != nullptr)
	{
		if (bIsCanSeeUI == true)
		{
			if (ParentCharacter->WasRecentlyRendered() == false)
			{
				EnemyUI->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				EnemyUI->SetVisibility(ESlateVisibility::Visible);
			}
			EnemyUI->SetUpdateHPGaugePosition(ParentCharacter);
		}
		else
		{
			EnemyUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

/////////////////////////////////////////////
// Move

FVector IEnemyInterface::GetNewLocation(FVector InLocation)
{
	float MoveableWidth = FMath::RandRange(-500, 500);
	float MoveableHeight = FMath::RandRange(-500, 500);
	float PreLocationX = InLocation.X + MoveableWidth;
	float PreLocationY = InLocation.Y + MoveableHeight;
	return FVector(PreLocationX, PreLocationY, InLocation.Z);
}

void IEnemyInterface::MoveSafety()
{
	if (AIController != nullptr)
	{
		if (Player != nullptr)
		{
			FVector TargetLocation = Player->GetActorLocation();
			for (const FVector& MovableLoc : SafetyMovable)
			{
				MoveResult = MoveToLocation(TargetLocation + MovableLoc);
				if (MoveResult == EPathFollowingRequestResult::RequestSuccessful)
				{
					return;
				}
			}
		}
	}
}

EPathFollowingRequestResult::Type IEnemyInterface::MoveToLocation(FVector InLocation)
{
	if (AIController != nullptr)
	{
		// if bUseFinding is false. the character move to InLocation whether Navigation Mesh is exists
		//return AIController->MoveToLocation(InLocation, -1.f, true, false);
		return AIController->MoveToLocation(InLocation, -1.f, true, true);

	}
	return EPathFollowingRequestResult::Failed;
}

EPathFollowingRequestResult::Type IEnemyInterface::MoveToTarget()
{
	if (AIController != nullptr)
	{
		if (Player != nullptr)
		{
			//return AIController->MoveToActor(Player, -1.f, true, false);
			return AIController->MoveToActor(Player, 150.f, true, true);
		}
	}
	return EPathFollowingRequestResult::Failed;
}

/////////////////////////////////////////////
// Status

void IEnemyInterface::UpdateHealth(float ChangeHealth)
{
	Health += ChangeHealth;
	Health = FMath::Clamp(Health, 0.f, FullHealth);
	HealthPercentage = Health / FullHealth;
	HealthType = UpdateHealthType(HealthPercentage);
	if (EnemyUI != nullptr)
	{
		EnemyUI->UpdateHealth(HealthPercentage);
	}
}

EHealthType IEnemyInterface::UpdateHealthType(const float InHealthPercentage)
{
	if (InHealthPercentage > 0.5f)
	{
		return EHealthType::Full;
	}
	if (InHealthPercentage > 0.f && InHealthPercentage <= 0.5f)
	{
		return EHealthType::Half;
	}
	if (InHealthPercentage == 0.f)
	{
		return EHealthType::Zero;
	}

	return EHealthType::None;
}


///////////////////////////////////////////////
// Animation

void IEnemyInterface::PlayAnimation(UAnimMontage* PlayMontage)
{
	AnimInstance = MeshComp->GetAnimInstance();

	if (AnimInstance != nullptr)
	{
		if (PlayMontage != nullptr)
		{
			UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
			if (CurrentMontage != nullptr)
			{
				FString AnimName = CurrentMontage->GetName();
				TArray<FString> res;
				AnimName.ParseIntoArray(res, TEXT("_"));
				if (res.Num() > 1)
				{
					if (res[1] == "DeathMontage")
					{
						return;
					}
				}
				if (AnimInstance->IsAnyMontagePlaying() == true)
				{
					AnimInstance->StopAllMontages(1.f);
				}
			}
			AnimInstance->Montage_Play(PlayMontage);
		}
	}
}

bool IEnemyInterface::IsPlayingAnim(UAnimMontage* PlayMontage)
{
	if (PlayMontage == nullptr || AnimInstance == nullptr) return false;
	return AnimInstance->Montage_IsPlaying(PlayMontage);
}

void IEnemyInterface::StopAnimation(UAnimMontage* PlayMontage)
{
	if (PlayMontage != nullptr)
	{
		AnimInstance->Montage_Stop(1.f, PlayMontage);
	}
	else
	{
		AnimInstance->StopAllMontages(1.f);
	}
}

/////////////////////////////////////////////
// Math

FRotator IEnemyInterface::FindLookAtRotation(const FVector& Start, const FVector& Target)
{
	return MakeRotFromX(Target - Start);
}

FRotator IEnemyInterface::MakeRotFromX(const FVector& X)
{
	return FRotationMatrix::MakeFromX(X).Rotator();
}

/////////////////////////////////////////////
// 

URoguelikeGameInstance* IEnemyInterface::GetGameInstance()
{
	return GameInstance = URoguelikeGameInstance::GetGameInstance();
}