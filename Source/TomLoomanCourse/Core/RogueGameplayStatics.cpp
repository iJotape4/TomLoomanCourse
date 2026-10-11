// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueGameplayStatics.h"

#include "TomLoomanCourse/SharedGameplayTags.h"
#include "TomLoomanCourse/ActionSystem/URogueActionSystemComponent.h"

bool URogueGameplayStatics::IsFullHealth(URogueActionSystemComponent* ActionSystemComponent)
{
	FRogueAttribute* Health = ActionSystemComponent->GetAttribute(SharedGameplayTags::Attribute_Health);
	FRogueAttribute* HealthMax = ActionSystemComponent->GetAttribute(SharedGameplayTags::Attribute_HealthMax);
	
	return FMath::IsNearlyEqual(Health->GetValue(), HealthMax->GetValue());
}