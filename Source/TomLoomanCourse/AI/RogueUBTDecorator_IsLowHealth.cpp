// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueUBTDecorator_IsLowHealth.h"

#include "AIController.h"
#include "TomLoomanCourse/SharedGameplayTags.h"
#include "TomLoomanCourse/ActionSystem/URogueActionSystemComponent.h"

bool URogueUBTDecorator_IsLowHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                uint8* NodeMemory) const
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	check(Pawn);

	URogueActionSystemComponent* AttributesComponent = Cast<URogueActionSystemComponent>(Pawn->GetComponentByClass(URogueActionSystemComponent::StaticClass()));
	if (!ensure(AttributesComponent))
		return false;

	const float Health = AttributesComponent->GetAttribute(SharedGameplayTags::Attribute_Health)->GetValue();
	const float HealthMax = AttributesComponent->GetAttribute(SharedGameplayTags::Attribute_HealthMax)->GetValue();  
	
	return Health / HealthMax <= LowHealthThreshold;
}
