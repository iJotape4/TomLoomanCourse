// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction.h"

#include "URogueActionSystemComponent.h"

void URogueAction::StartAction_Implementation()
{
	float GameTime = GetWorld()->TimeSeconds;
	CooldownUntil  = GameTime + CooldownTime;
	UE_LOGFMT(LogTemp, Log, "Started Action {ActionName} - {WorldTime}", ActionName.ToString(), GameTime);
}

void URogueAction::StopAction_Implementation()
{
	float GameTime = GetWorld()->TimeSeconds;
	UE_LOGFMT(LogTemp, Log, "Stopped Action {ActionName} - {WorldTime}", ActionName.ToString(), GameTime);

}

bool URogueAction::CanStartAction() const
{
	if (GetCoolDownTimeRemaining() > 0.0)
	{
		UE_LOGFMT(LogTemp, Log, "{ActionName} Cooldown remaining : {cooldown}", ActionName.ToString(),GetCoolDownTimeRemaining());
		return false;
	}

	return true;
}

float URogueAction::GetCoolDownTimeRemaining() const
{
	return FMath::Max(0.0f, CooldownUntil - GetWorld()->TimeSeconds);
}

URogueActionSystemComponent* URogueAction::GetOwningComponent() const
{
	return Cast<URogueActionSystemComponent>(GetOuter());
}
