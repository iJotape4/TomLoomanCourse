// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributesComponent.h"

// Sets default values for this component's properties
USAttributesComponent::USAttributesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Health = MaxHealth;
}


// Called when the game starts
void USAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool USAttributesComponent::ApplyHealthChange(float Delta)
{
	if (Delta == 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to do damage Actor %s but projectile damage is zero"), *GetOwner()->GetActorLabel());
		return false;
	}
	float temp = Health + Delta;
	if (temp >= MaxHealth)
		Health = MaxHealth;
	else if (temp<=0)
	{
		Health = 0.0f;
		Death();
	}
	else
	{
		Health += Delta;
	}
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return true;
}

void USAttributesComponent::Death()
{
	if (!bIsAlive) return;
	OnDeath.Broadcast(nullptr);
	UE_LOG(LogTemp, Warning, TEXT("Health of Actor %s has reached Zero"), *GetOwner()->GetActorLabel());
	bIsAlive = true;
}


// Called every frame
void USAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USAttributesComponent::IsAlive() const
{
	return bIsAlive;
}

