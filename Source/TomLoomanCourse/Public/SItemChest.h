// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

class UNiagaraComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASItemChest : public AActor, public ISGameplayInterface
{
	
	GENERATED_BODY()
public:
	ASItemChest();

	virtual void PostInitializeComponents() override;
	UFUNCTION()
	virtual void LidOpeningUpdate(float Alpha);
	UFUNCTION()
	virtual void LidOpeningFinished();
	
	virtual void ToggleOpen();
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual void Tick(float DeltaTime) override;
    	
protected:
	UPROPERTY(VisibleAnywhere, Category= "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category= "Components")
	UStaticMeshComponent* LidMesh;

	UPROPERTY(VisibleAnywhere, Category= "Components")
	UStaticMeshComponent* GoldPile;

	UPROPERTY(VisibleAnywhere, Category= "Components")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Config")
	UCurveFloat* LidOpeningCurve = nullptr;

	UPROPERTY(VisibleAnywhere, Category= "Config")
	FTimeline LidOpeningTimeline;
	
	UPROPERTY(EditAnywhere, Category= "Config")	
	float LidOpenTargetPitch;

	UPROPERTY(VisibleAnywhere, Category="State")
	bool bIsOpen = false;
};