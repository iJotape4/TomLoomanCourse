// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

struct FAIStimulus;
class UAIPerceptionComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FName TargetActorKeyName = "TargetActor";
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPerceptionComponent* AIPerceptionComponent;

public:
	// Sets default values for this character's properties
	ASAICharacter();
	
	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
