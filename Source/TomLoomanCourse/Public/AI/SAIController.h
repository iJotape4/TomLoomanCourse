// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "SAIController.generated.h"
 
UCLASS()
class TOMLOOMANCOURSE_API ASAIController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Config")
	UBehaviorTree* BehaviorTree;
	
	// Sets default values for this actor's properties
	ASAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
