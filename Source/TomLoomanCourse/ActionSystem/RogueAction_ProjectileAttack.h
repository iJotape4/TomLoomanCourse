// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueAction.h"
#include "RogueAction_ProjectileAttack.generated.h"

class ARogueProjectileBase;
class UNiagaraSystem;
/**
 * 
 */
UCLASS(Abstract)
class TOMLOOMANCOURSE_API URogueAction_ProjectileAttack : public URogueAction
{
	GENERATED_BODY()
public:
	URogueAction_ProjectileAttack();
	
private:	
	virtual  void PostInitProperties() override;
	virtual void StartAction_Implementation() override;
	void AttackTimerElapsed() const;
	FVector GetHandLocation() const;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileAttack")
	TSubclassOf<ARogueProjectileBase> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "ProjectileAttack")
	TObjectPtr<UNiagaraSystem> CastingEffect;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileAttack")
	TObjectPtr<USoundBase> CastingSound;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileAttack")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileAttack")
	TObjectPtr<UAnimMontage> AttackMontage;
	float MaxRange = 5000.0f;
	
	TObjectPtr<ACharacter> Character;
};
