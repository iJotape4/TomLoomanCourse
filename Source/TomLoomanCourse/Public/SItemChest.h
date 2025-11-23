// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

UCLASS()
class TOMLOOMANCOURSE_API ASItemChest : public AActor, public ISGameplayInterface
{
	
	GENERATED_BODY()
public:
	ASItemChest();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
    	
protected:
	UPROPERTY(VisibleAnywhere, Category= "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category= "Components")
	UStaticMeshComponent* LidMesh;

	UPROPERTY(VisibleAnywhere, Category= "Components")
	UStaticMeshComponent* GoldPile;


	UPROPERTY(EditAnywhere, Category= "Config")	
	float LidMeshTargetPitch;
};