// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributesComponent*, OwningComp,  float, InHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, InstigatorActor);
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOMLOOMANCOURSE_API USAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USAttributesComponent();
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly , Category = "Attributes")
	float Health = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly , Category = "Attributes")
	float MaxHealth = 100.0f;

	bool bIsAlive =true;
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void Death();
	
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,	
	                           FActorComponentTickFunction* ThisTickFunction) override;

};