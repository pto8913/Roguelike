// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/UMG/EnemyWidget.h"
#include "Public/RoguelikeGameInstance.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "ProgressBar.h"

void UEnemyWidget::PreConstruct()
{
	SetAlignmentInViewport(FVector2D(0.7f, 0.7f));
}

void UEnemyWidget::OpenEnemyUI(float NewHealth)
{
	if (!IsInViewport())
	{
		AddToViewport();
		UpdateHealth(NewHealth);

		GameInstance = URoguelikeGameInstance::GetGameInstance();
	}
}

void UEnemyWidget::CloseEnemyUI()
{
	if (IsInViewport() == true)
	{
		RemoveFromParent();
	}
}

void UEnemyWidget::UpdateHealth(float NewHealth)
{
	if (EnemyHealth != nullptr)
	{
		EnemyHealth->SetPercent(NewHealth);
	}
}

void UEnemyWidget::SetUpdateHPGaugePosition(ACharacter* ParentCharacter)
{
	if (GameInstance != nullptr)
	{
		if (ParentCharacter != nullptr)
		{
			APlayerController* PlayerController = GameInstance->GetPlayerController();
			if (PlayerController != nullptr)
			{
				FVector HeadBoneLocation = ParentCharacter->GetMesh()->GetBoneLocation("head");

				FVector2D x1, x2;
				bool bx1 = PlayerController->ProjectWorldLocationToScreen(
					HeadBoneLocation + 50.f, x1
				);

				bool bx2 = PlayerController->ProjectWorldLocationToScreen(
					HeadBoneLocation - 50.f, x2
				);
				if (bx1 == true && bx2 == true)
				{
					float tmp = (x1.X - x2.X) / 400;
					tmp = FMath::Clamp(tmp, 0.2f, 1.f);
					SetRenderScale(FVector2D(tmp, tmp));
				}

				HeadBoneLocation.Z += 50.f;
				HeadBoneLocation.Y -= 70.f;
				FVector2D ScreenLocation = FVector2D::ZeroVector;
				PlayerController->ProjectWorldLocationToScreen(
					HeadBoneLocation, ScreenLocation
				);
				SetPositionInViewport(ScreenLocation);
			}
		}
	}
	else
	{
		GameInstance = URoguelikeGameInstance::GetGameInstance();
	}
}
