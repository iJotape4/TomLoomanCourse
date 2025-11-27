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

	// ...
	
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
		Health =0.0f;
		Dead();
		return false;
	}
	else
	{
		Health += Delta;
	}
	UE_LOG(LogTemp, Warning, TEXT("Health changed by %f, new health: %f"), Delta, Health);
	return  true;
}

void USAttributesComponent::Dead()
{
	UE_LOG(LogTemp, Warning, TEXT("You're Dead"));
}


// Called every frame
void USAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

