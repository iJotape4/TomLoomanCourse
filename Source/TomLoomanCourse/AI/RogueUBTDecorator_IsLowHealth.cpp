// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueUBTDecorator_IsLowHealth.h"

#include "AIController.h"
#include "SAttributesComponent.h"

bool URogueUBTDecorator_IsLowHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                uint8* NodeMemory) const
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	check(Pawn);

	USAttributesComponent* AttributesComponent = Cast<USAttributesComponent>(Pawn->GetComponentByClass(USAttributesComponent::StaticClass()));
	if (!ensure(AttributesComponent))
		return false;

	return AttributesComponent->GetHealthPercent() <= LowHealthThreshold;
}
