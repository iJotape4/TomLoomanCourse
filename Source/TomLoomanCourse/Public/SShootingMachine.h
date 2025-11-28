// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SShootingMachine.generated.h"

class ASProjectileBase;
class UArrowComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASShootingMachine : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Components");
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Config | Projectile");
	TSubclassOf<ASProjectileBase> Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Config | Behavior");
	bool bShouldTrackPlayer = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Config | Behavior");
	float ProjectileSpawnRate = 2.0f;
	FTimerHandle Timer;
	// Sets default values for this actor's properties
	ASShootingMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SpawnProjectile();
	bool FindPlayer(FVector& OutLocation, FRotator& OutDirection);
};
