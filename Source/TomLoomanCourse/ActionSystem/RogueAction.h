// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	FName ActionName = FName("PrimaryAttack");

	// Attacks are the actions the player cycles through with the switch weapon input
	UPROPERTY(EditDefaultsOnly, Category="Actions")
	bool bIsAttack = false;
	float CooldownTime =0.0f;

public:
	bool CanStartAction() const;
	UFUNCTION(BlueprintNativeEvent, Category="Actions")
	void StartAction();

	UFUNCTION(BlueprintNativeEvent, Category="Actions")
	void StopAction();

	float GetCoolDownTimeRemaining () const;
	UFUNCTION(BlueprintCallable)
	URogueActionSystemComponent* GetOwningComponent() const;
	
	FName GetActionName() const { return ActionName; }
	bool IsAttack() const { return bIsAttack; }
	UPROPERTY(Transient)
	float CooldownUntil = 0;
};
