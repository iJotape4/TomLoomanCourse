// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTTask_Heal.h"

#include "AIController.h"
#include "RogueAICharacter.h"
#include "RogueAIController.h"

EBTNodeResult::Type URogueBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARogueAICharacter* Character = Cast<ARogueAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!ensure(Character))
		return EBTNodeResult::Failed;

	return Character->AttributesComponent->ApplyHealthChange(HealAmount) ?  EBTNodeResult::Succeeded :  EBTNodeResult::Failed;  
}
