// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributesComponent.h"

// Sets default values for this component's properties
USAttributesComponent::USAttributesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	//This line of code set the healthbar Widget to MaxHealth at BeginPlay
	ApplyHealthChange(0);
}

bool USAttributesComponent::ApplyHealthChange(float Delta)
{

	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	if (Health == 0)
		Death();

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

