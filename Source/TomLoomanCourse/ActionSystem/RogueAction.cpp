// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction.h"

#include "URogueActionSystemComponent.h"

void URogueAction::StartAction_Implementation()
{
	float GameTime = GetWorld()->TimeSeconds;
	UE_LOGFMT(LogTemp, Log, "Started Action {ActionName} - {WorldTime}", ActionName, GameTime); 
}

void URogueAction::StopAction_Implementation()
{
	float GameTime = GetWorld()->TimeSeconds;
	UE_LOGFMT(LogTemp, Log, "Stopped Action {ActionName} - {WorldTime}", ActionName, GameTime); 
}

URogueActionSystemComponent* URogueAction::GetOwningComponent() const
{
	return Cast<URogueActionSystemComponent>(GetOuter());
}
