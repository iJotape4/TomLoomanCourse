// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerHealthBar.h"

#include "GameplayTagContainer.h"

void USPlayerHealthBar::SetHealth(FGameplayTag GameplayTag, float NewHealth, float OldHealth)
{
	Health = NewHealth;
	OnHealthChanged(nullptr, nullptr, NewHealth, NewHealth-OldHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health changed to %f"), Health);
}

float USPlayerHealthBar::GetHealth() const
{
	return Health;
}