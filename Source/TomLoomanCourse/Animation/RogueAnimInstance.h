// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RogueAnimInstance.generated.h"

class URogueActionSystemComponent;
/**
 * 
 */
UCLASS()
class TOMLOOMANCOURSE_API URogueAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	
protected:
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Config | Health")
	bool bIsAlive = true;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "StatusEffects")
	bool bIsSprinting;
	
protected:
	
	UPROPERTY(Transient, BlueprintReadOnly)
	TObjectPtr<URogueActionSystemComponent> Actioncomp;
	
public:
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Death(AActor* InstigatorActor);
};