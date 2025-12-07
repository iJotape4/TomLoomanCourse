// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	//Check distance between AI Pawn and target actor
	 UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		if (AActor* TargetActor =  Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor")))
		{
			AAIController* Controller = OwnerComp.GetAIOwner();
			if (ensure(Controller))
			{
				APawn* AIPawn  = Controller -> GetPawn();
				if (ensure(AIPawn))
				{
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					 bool bInAttackRange = DistanceTo <= 2000.f;

					if (!bInAttackRange) return;

					bool bHasLOS = Controller->LineOfSightTo(TargetActor);
					
					BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bInAttackRange && bHasLOS));
				}
			}
		}
	}
	
}
