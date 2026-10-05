// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPlayerHealthBar.generated.h"

class URogueActionSystemComponent;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = UI)
class TOMLOOMANCOURSE_API USPlayerHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HealthWidget")
	void SetHealth(AActor* InstigatorActor, URogueActionSystemComponent* OwningComp,  float InHealth, float Delta);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "HealthWidget")
	void OnHealthChanged(AActor* InstigatorActor, URogueActionSystemComponent* OwningComp, const float NewHealth, float Delta);

	UFUNCTION(BlueprintCallable, Category = "HealthWidget	")
	float GetHealth() const;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "HealthWidget")
	void SetDefaults(URogueActionSystemComponent* OwningComp);

private:
	UPROPERTY(Transient)
	float Health = 100.0f;
};
