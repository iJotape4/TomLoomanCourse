// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARogueProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;
class USoundBase;

UCLASS(Abstract)
class TOMLOOMANCOURSE_API ARogueProjectileBase : public AActor
{
	GENERATED_BODY()

public:

	ARogueProjectileBase();

	virtual void PostInitializeComponents() override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Animation")
	float LifeTime = 5.0f;

	/* Spawned at the projectile's location when it explodes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Animation")
	TObjectPtr<UNiagaraSystem> EmitterOnCrash;

	/* Looped while the projectile is in flight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | SFX")
	TObjectPtr<USoundBase> FlightSFX;

	/* Played at the projectile's location when it explodes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | SFX")
	TObjectPtr<USoundBase> ImpactSFX;

	virtual void BeginPlay() override;

	/* Default behavior: explode and destroy. Override in child classes to add or replace behavior. */
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/* VFX, SFX and camera shake at the projectile's current location */
	void PlayExplodeEffects();
};
