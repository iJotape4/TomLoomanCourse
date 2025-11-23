// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "SEnemy.generated.h"

class USplineComponent;
class UPawnSensingComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USplineComponent* SplineComponent;
	
public:
	// Sets default values for this actor's properties
	ASEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
