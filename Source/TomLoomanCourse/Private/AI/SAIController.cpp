// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

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

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller")))
		RunBehaviorTree(BehaviorTree);
}