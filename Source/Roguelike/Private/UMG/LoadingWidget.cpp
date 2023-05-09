// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/UMG/LoadingWidget.h"
#include "Engine.h"

void ULoadingWidget::OpenLoad()
{
	if (!IsInViewport())
	{
		AddToViewport();
	}
}


void ULoadingWidget::CloseLoad()
{
	if (IsInViewport() == true)
	{
		RemoveFromParent();
	}
}
