// Fill out your copyright notice in the Description page of Project Settings.


#include "TomLoomanCourse/ActionSystem/URogueActionSystemComponent.h"

#include "RogueAction.h"

// Sets default values for this component's properties
URogueActionSystemComponent::URogueActionSystemComponent()
{
	bWantsInitializeComponent = true;
}

// Called when the game starts
void URogueActionSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	OnBeginPlay.Broadcast(this);
}

void URogueActionSystemComponent::InitializeComponent()
{
	Super::InitializeComponent();
	for (TSubclassOf<URogueAction> ActionClass : DefaultActions)
		if (ensure(ActionClass))
		GrantAction(ActionClass);

	for (TSubclassOf<URogueAction> ActionClass : AttackActions)
		if (ensure(ActionClass))
			AttackInstances.Add(GrantAction(ActionClass));
}

void URogueActionSystemComponent::StartAction(FName InActionName)
{
	for (URogueAction* Action : Actions)
	{
		if (Action->GetActionName() == InActionName)
		{
			Action->StartAction();
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Action %s not found"), *InActionName.ToString());
}

void URogueActionSystemComponent::StartAction(int ActionIndex)
{
	if (AttackInstances.IsValidIndex(ActionIndex))
		AttackInstances[ActionIndex]->StartAction();
	else
		UE_LOG(LogTemp, Warning, TEXT("No attack action at index %d"), ActionIndex);
}

void URogueActionSystemComponent::StopAction(FName InActionName)
{
	for (URogueAction* Action : Actions)
	{
		if (Action->GetActionName() == InActionName)
		{
			Action->StopAction();
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Action %s not found"), *InActionName.ToString());
}

URogueAction* URogueActionSystemComponent::GrantAction(const TSubclassOf<URogueAction>& NewActionClass)
{
	URogueAction* NewAction = NewObject<URogueAction>(this, NewActionClass);
	Actions.Add(NewAction);
	return NewAction;
}

bool URogueActionSystemComponent::ApplyHealthChange(float Delta)
{

	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	if (Health == 0)
	{
		Death();
		return  false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Health of Actor %s has changed to %f"), *GetOwner()->GetActorLabel(), Health);
		return true;
	}

}

bool URogueActionSystemComponent::IsFullHealth() const
{
	return Health >= MaxHealth;
}

void URogueActionSystemComponent::Death()
{
	if (!bIsAlive) return;
	OnDeath.Broadcast(nullptr);
	UE_LOG(LogTemp, Warning, TEXT("Health of Actor %s has reached Zero"), *GetOwner()->GetActorLabel());
	bIsAlive = false;
}

float URogueActionSystemComponent::GetHealthPercent() const
{
	return  Health / MaxHealth;
}

bool URogueActionSystemComponent::IsAlive() const
{
	return bIsAlive;
}

URogueActionSystemComponent* URogueActionSystemComponent::GetAttributesComponent(AActor* FromActor)
{
	return FromActor ? FromActor->FindComponentByClass<URogueActionSystemComponent>() : nullptr;
}

