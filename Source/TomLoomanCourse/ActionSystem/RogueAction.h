// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "RogueAction.generated.h"

class URogueActionSystemComponent;
/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class TOMLOOMANCOURSE_API URogueAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="Actions")
	FGameplayTag ActionName;

	UPROPERTY(EditDefaultsOnly, Category="Actions")
	float CooldownTime =0.0f;
	// Attacks are the actions the player cycles through with the switch weapon input

public:
	bool CanStartAction() const;
	
	UFUNCTION(BlueprintNativeEvent, Category="Actions")
	void StartAction();

	UFUNCTION(BlueprintNativeEvent, Category="Actions")
	void StopAction();

	float GetCoolDownTimeRemaining () const;

	UFUNCTION(BlueprintCallable)
	URogueActionSystemComponent* GetOwningComponent() const;
	
	FGameplayTag GetActionName() const { return ActionName; }

protected:
	
	UPROPERTY(Transient)
	float CooldownUntil = 0;
};
