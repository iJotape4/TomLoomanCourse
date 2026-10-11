// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTTask_Heal.h"
#include "AIController.h"
#include "TomLoomanCourse/SharedGameplayTags.h"
#include "TomLoomanCourse/ActionSystem/URogueActionSystemComponent.h"

EBTNodeResult::Type URogueBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	check(Pawn);

	URogueActionSystemComponent::GetAttributesComponent(Pawn)->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, HealAmount);
	return EBTNodeResult::Succeeded;  
}
