// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("inherited");
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ASAICharacter::OnTargetPerceptionUpdated);
	}
}

void ASAICharacter::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("Target updated %s"), *Actor->GetName());
	APawn* Pawn = Cast<APawn>(Actor);
	if (!Pawn) return;

	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		UBlackboardComponent* Blackboard = AIC->GetBlackboardComponent();
		if (Stimulus.WasSuccessfullySensed())
		{
			Blackboard->SetValueAsObject(TargetActorKeyName, Pawn);
			DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
		}
		else
		{
			// optionally clear target when lost:
			// Blackboard->ClearValue(TargetActorKeyName);
		}
	}
}