// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTTask_Heal.h"
#include "AIController.h"
#include "SAttributesComponent.h"

EBTNodeResult::Type URogueBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	check(Pawn);

	return USAttributesComponent::GetAttributesComponent(Pawn)->ApplyHealthChange(HealAmount) ?  EBTNodeResult::Succeeded :  EBTNodeResult::Failed;  
}
