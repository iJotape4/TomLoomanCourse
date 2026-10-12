// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction.h"

#include "URogueActionSystemComponent.h"

void URogueAction::StartAction_Implementation()
{
	URogueActionSystemComponent* OwningComp = GetOwningComponent();

	float GameTime = GetWorld()->TimeSeconds;
	CooldownUntil  = GameTime + CooldownTime;
	UE_LOGFMT(LogTemp, Log, "Started Action {ActionName} - {WorldTime}", ActionName.ToString(), GameTime);
	
	OwningComp->ActiveGameplayTags.AppendTags(GrantTags);
	
	for (TPair<FGameplayTag, float > Cost : ActivationCost)
		OwningComp->ApplyAttributeChange(Cost.Key, -Cost.Value, Modifier);
}

void URogueAction::StopAction_Implementation()
{
	float GameTime = GetWorld()->TimeSeconds;
	UE_LOGFMT(LogTemp, Log, "Stopped Action {ActionName} - {WorldTime}", ActionName.ToString(), GameTime);
	
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantTags);
}

bool URogueAction::CanStartAction() const
{
	if (GetCoolDownTimeRemaining() > 0.0)
	{
		UE_LOGFMT(LogTemp, Log, "{ActionName} Cooldown remaining : {cooldown}", ActionName.ToString(),GetCoolDownTimeRemaining());
		return false;
	}
	
	URogueActionSystemComponent* OwningComp = GetOwningComponent();
	
	if (OwningComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false; 
	}
	
	for (TPair<FGameplayTag, float> Cost : ActivationCost)
	{
		float AvailableAttributeAmount = OwningComp->GetAttributeValue(Cost.Key);
		if (Cost.Value > AvailableAttributeAmount)
		{
			UE_LOGFMT(LogTemp, Log, "{ActionName} Not enough {AttributeTag} to activate", ActionName.ToString(), Cost.Key.ToString());
			return false;
		}
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
