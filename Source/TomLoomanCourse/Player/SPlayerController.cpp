// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "TomLoomanCourse/ActionSystem/URogueActionSystemComponent.h"
#include "SPlayerHealthBar.h"
#include "TomLoomanCourse/SharedGameplayTags.h"

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
	
	AttributesComponent = InPawn->FindComponentByClass<URogueActionSystemComponent>();
	if (AttributesComponent && HealthBarWidget)
	{
		AttributesComponent->OnBeginPlay.AddDynamic(HealthBarWidget, &USPlayerHealthBar::SetDefaults);
		AttributesComponent->GetAttributeListener(SharedGameplayTags::Attribute_Health)
			.AddUObject(HealthBarWidget, &USPlayerHealthBar::SetHealth);		
		AttributesComponent->OnDeath.AddDynamic( this, &ASPlayerController::HandleOnPawnDeath ) ; 
	}

}

void ASPlayerController::OnUnPossess()
{
	if (AttributesComponent)
	{
		if (HealthBarWidget)
		{
			AttributesComponent->GetAttributeListener(SharedGameplayTags::Attribute_Health).RemoveAll(HealthBarWidget);
			AttributesComponent->OnBeginPlay.RemoveDynamic(HealthBarWidget, &USPlayerHealthBar::SetDefaults);
		}
		AttributesComponent->OnDeath.RemoveDynamic(this, &ASPlayerController::HandleOnPawnDeath);
		AttributesComponent = nullptr;
	}

	Super::OnUnPossess();
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