// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueAICharacter.generated.h"

class USAttributesComponent;

UCLASS()
class TOMLOOMANCOURSE_API ARogueAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARogueAICharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USAttributesComponent> AttributesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsLowHealth = false;
};