// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

UCLASS()
class TOMLOOMANCOURSE_API ASItemChest : public AActor, public ISGameplayInterface
{
	
	GENERATED_BODY()
public:
	ASItemChest();

	virtual void PostInitializeComponents() override;
	UFUNCTION()
	virtual void OpeningUpdate(float Alpha);
	UFUNCTION()
	virtual void OpeningFinished();
	
	virtual void StartOpen();
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual void Tick(float DeltaTime) override;
    	
protected:
	UPROPERTY(VisibleAnywhere, Category= "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category= "Components")
	UStaticMeshComponent* LidMesh;

	UPROPERTY(VisibleAnywhere, Category= "Components")
	UStaticMeshComponent* GoldPile;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Config")
	UCurveFloat* OpeningCurve = nullptr;

	UPROPERTY(VisibleAnywhere, Category= "Config")
	FTimeline OpeningTimeline;
	
	UPROPERTY(EditAnywhere, Category= "Config")	
	float LidMeshTargetPitch;

	UPROPERTY(VisibleAnywhere, Category="State")
	bool bIsOpen = false;
};