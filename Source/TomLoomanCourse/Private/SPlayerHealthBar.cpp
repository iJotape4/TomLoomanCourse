// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerHealthBar.h"

void USPlayerHealthBar::SetHealth(AActor* InstigatorActor, USAttributesComponent* OwningComp, const float InHealth, float Delta)
{
	Health = InHealth;
	OnHealthChanged(InstigatorActor, OwningComp, InHealth, Delta);
	UE_LOG(LogTemp, Warning, TEXT("Health changed to %f"), Health);
}

float USPlayerHealthBar::GetHealth() const
{
	return Health;
}