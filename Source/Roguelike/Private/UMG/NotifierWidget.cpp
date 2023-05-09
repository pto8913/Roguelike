// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/UMG/NotifierWidget.h"
#include "Public/RoguelikeGameInstance.h"

#include "Button.h"

void UNotifierWidget::OpenNotifier()
{
	if (!IsInViewport())
	{
		if (GetGameInstance() != nullptr)
		{
			AddToViewport(10);
			AddDelegate();
			GameInstance->GamePause(FInputModeUIOnly());
		}
	}
}


void UNotifierWidget::CloseNotifier()
{
	if (IsInViewport())
	{
		if (GetGameInstance() != nullptr)
		{
			RemoveFromParent();
			RemoveDelegate();
			GameInstance->GameUnPause(FInputModeGameOnly());
		}
	}
}


////////////////////////////////////////////////////////////
//

void UNotifierWidget::AddDelegate()
{
	ButtonClose->OnClicked.AddDynamic(this, &UNotifierWidget::CloseNotifier);
}


void UNotifierWidget::RemoveDelegate()
{
	ButtonClose->OnClicked.RemoveDynamic(this, &UNotifierWidget::CloseNotifier);
}


URoguelikeGameInstance* UNotifierWidget::GetGameInstance()
{
	return GameInstance = URoguelikeGameInstance::GetGameInstance();
}