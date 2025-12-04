// Fill out your copyright notice in the Description page of Project Settings.


#include "SDamagePopUp_Widget.h"

void USDamagePopUp_Widget::Display()
{
	if (AttachTo)
	AddToViewport();
}

void USDamagePopUp_Widget::OnTimeElapsed()
{
	RemoveFromParent();
}

void USDamagePopUp_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USDamagePopUp_Widget::OnTimeElapsed, TimeOnScreen);
}

void USDamagePopUp_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (AttachTo)
	{
		FVector2D ScreenPosition;
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC && PC->ProjectWorldLocationToScreen(AttachTo->GetActorLocation(), ScreenPosition))
		{
			SetPositionInViewport(ScreenPosition);
		}
	}
}