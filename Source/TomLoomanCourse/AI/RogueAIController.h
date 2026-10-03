// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAttributesComponent.h"
#include "RogueAIController.generated.h"

class ARogueAICharacter;
class UBehaviorTree;

UCLASS()
class TOMLOOMANCOURSE_API ARogueAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARogueAIController();

protected:
	UPROPERTY(EditDefaultsOnly, Category="AI")	
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void HandleHealthChange(AActor* InstigatorActor, USAttributesComponent* OwningComp, float InHealth, float Delta);
	
	UFUNCTION()
	void HandleDeath(AActor* InstigatorActor);
	
	virtual void FleeOnLowHealth();
	virtual void ComeBackOnHealed();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ARogueAICharacter> SelfPawn;
	
	FName LowHealthKey = FName("LowHealth");

	bool bLowHealth = false;
};
