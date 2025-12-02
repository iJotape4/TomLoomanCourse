// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "SAttributesComponent.h"
#include "SPlayerHealthBar.h"

void ASPlayerController::HandleOnPawnDeath(AActor* InstigatorActor)
{
	OnUnPossess();
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ASPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!IsLocalController()) return;

	if (HealthBarWidget == nullptr && IsValid(HealthBarClass))
	{
		HealthBarWidget = CreateWidget<USPlayerHealthBar>(this, HealthBarClass);
		if (HealthBarWidget)
		{
			HealthBarWidget->AddToViewport();
		}
	}
	
	AttributesComponent = InPawn->FindComponentByClass<USAttributesComponent>();
	if (AttributesComponent && HealthBarWidget)
	{
		AttributesComponent->OnHealthChanged.AddDynamic(HealthBarWidget, &USPlayerHealthBar::SetHealth);
		AttributesComponent->OnDeath.AddDynamic( this, &ASPlayerController::HandleOnPawnDeath ) ; 
	}

}

void ASPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	if (APawn* P = GetPawn())
	{
		if (USAttributesComponent* Attr = P->FindComponentByClass<USAttributesComponent>())
		{
			if (HealthBarWidget)
			{
				Attr->OnHealthChanged.RemoveDynamic(HealthBarWidget, &USPlayerHealthBar::SetHealth);
			}
		}
	}
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
}	