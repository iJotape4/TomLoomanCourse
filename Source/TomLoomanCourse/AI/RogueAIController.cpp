// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAIController.h"

#include "RogueAICharacter.h"
#include "SAttributesComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARogueAIController::ARogueAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARogueAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	FName TargetActor = FName("TargetActor");
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	check(PlayerPawn);
	
	GetBlackboardComponent()->SetValueAsObject(TargetActor, PlayerPawn);
}

void ARogueAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (InPawn)
	{
		SelfPawn = Cast<ARogueAICharacter>(InPawn);
		check(SelfPawn);
		SelfPawn->AttributesComponent->OnHealthChanged.AddDynamic(this, &ARogueAIController::HandleHealthChange);
		SelfPawn->AttributesComponent->OnDeath.AddDynamic(this, &ARogueAIController::HandleDeath);
	}
}

void ARogueAIController::HandleHealthChange(AActor* InstigatorActor, USAttributesComponent* OwningComp, float InHealth,
                                            float Delta)
{
	bool bIsLowHealth = OwningComp->GetHealthPercent() <= 0.3f;
	if ( bIsLowHealth && !bLowHealth)
		FleeOnLowHealth();
	else if (!bIsLowHealth && bLowHealth)
		ComeBackOnHealed();

	GetBlackboardComponent()->SetValueAsBool(LowHealthKey, bLowHealth);
}

void ARogueAIController::HandleDeath(AActor* InstigatorActor)
{
	SelfPawn->AttributesComponent->OnHealthChanged.RemoveDynamic(this, &ARogueAIController::HandleHealthChange);
	SelfPawn->AttributesComponent->OnDeath.RemoveDynamic(this, &ARogueAIController::HandleDeath);
	SelfPawn->Destroy();	
}

void ARogueAIController::FleeOnLowHealth()
{
	bLowHealth = true;
	UE_LOG(LogTemp, Warning, TEXT("Low Health"));
}

void ARogueAIController::ComeBackOnHealed()
{
	bLowHealth =false;
};