// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASAIController::ASAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);

	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
	}
}

// Called every frame
void ASAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

