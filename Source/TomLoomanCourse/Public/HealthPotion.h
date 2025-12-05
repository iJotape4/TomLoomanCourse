// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableBase.h"
#include "HealthPotion.generated.h"

class USphereComponent;

UCLASS()
class TOMLOOMANCOURSE_API AHealthPotion : public APickableBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Components")
	USphereComponent* SphereComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Config")
	float HealthAmount = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Debug")
	bool bDrawDebugCollisionSphere = false;
	
	// Sets default values for this actor's properties
	AHealthPotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual auto Tick(float DeltaTime) -> void override;
};
