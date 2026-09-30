// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTService_CheckRangeTo.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

void URogueBTService_CheckRangeTo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	
	UBlackboardComponent* BBcomp = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = Cast<AActor>( BBcomp->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (TargetActor == nullptr)
		return;
	
	FVector TargetLocation = TargetActor->GetActorLocation();
	AAIController* Controller = OwnerComp.GetAIOwner();
	APawn* OwningPawn = Controller->GetPawn();
	check(OwningPawn);
	 
	FVector OriginLocation = OwningPawn->GetActorLocation();

	float DistanceTo = FVector::Dist(TargetLocation, OriginLocation);

	bool bWithinRange = DistanceTo <= MaxAttackRange;

	bool bHasLOS = Controller->LineOfSightTo(TargetActor);
	
	BBcomp->SetValueAsBool(WithinRangeKey.SelectedKeyName, bWithinRange &&  bHasLOS);
}
