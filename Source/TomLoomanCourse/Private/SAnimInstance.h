// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

class USAttributesComponent;
/**
 * 
 */
UCLASS()
class TOMLOOMANCOURSE_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Death(AActor* InstigatorActor);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config | Health")
	bool bIsAlive = true;
};