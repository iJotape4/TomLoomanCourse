// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPlayerHealthBar.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = UI)
class TOMLOOMANCOURSE_API USPlayerHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HealthWidget")
	void SetHealth(float InHealth);
	UFUNCTION(BlueprintImplementableEvent, Category = "HealthWidget")
	void OnHealthChanged(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "HealthWidget	")
	float GetHealth() const;

private:
	UPROPERTY(Transient)
	float Health = 100.0f;
};
