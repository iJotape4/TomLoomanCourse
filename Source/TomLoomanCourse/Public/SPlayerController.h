// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class USAttributesComponent;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class TOMLOOMANCOURSE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attributes")
	TObjectPtr<USAttributesComponent> AttributesComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class USPlayerHealthBar> HealthBarClass;

	UPROPERTY(VisibleAnywhere,Category="UI")
	USPlayerHealthBar* HealthBarWidget;

	virtual void BeginPlay() override;
	virtual  void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;
};
