// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerHealthBar.h"

void USPlayerHealthBar::SetHealth(const float InHealth)
{
	Health = InHealth;
	OnHealthChanged(Health);
	UE_LOG(LogTemp, Warning, TEXT("Health changed to %f"), Health);
}

float USPlayerHealthBar::GetHealth() const
{
	return Health;
}
