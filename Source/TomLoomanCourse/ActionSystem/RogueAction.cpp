// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction.h"

#include "URogueActionSystemComponent.h"

void URogueAction::StartAction()
{
	UE_LOGFMT(LogTemp, Log, "Started Action {ActionName}", ActionName); 
}

URogueActionSystemComponent* URogueAction::GetOwningComponent() const
{
	return Cast<URogueActionSystemComponent>(GetOuter());
}
