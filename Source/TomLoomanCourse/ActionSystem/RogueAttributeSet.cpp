// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAttributeSet.h"

void URogueHealthAttributeSet::PostAttributeChanged()
{
	Health.Base = FMath::Clamp(Health.Base, 0.0f, HealthMax.GetValue());
	if (Health.Base == 0)
	{
		//OnDeath.Broadcast(nullptr);
	}
}

URogueHealthAttributeSet::URogueHealthAttributeSet()
{
	Health = FRogueAttribute(100);
	HealthMax = FRogueAttribute(Health.GetValue());
}