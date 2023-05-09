// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/UMG/PlayerWidget.h"

void UPlayerWidget::OpenPlayerUI(float NewHealth)
{
	if (!IsInViewport())
	{
		AddToViewport();
		UpdateHealth(NewHealth);
	}
}

void UPlayerWidget::ClosePlayerUI()
{
	if (IsInViewport() == true)
	{
		RemoveFromParent();
	}
}

void UPlayerWidget::UpdateHealth(float NewHealth)
{
	if (PlayerHealth != nullptr)
	{
		PlayerHealth->SetPercent(NewHealth);
	}
}