// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARogueProjectileBase.h"
#include "STeleportProjectile.generated.h"

UCLASS(Abstract)
class TOMLOOMANCOURSE_API ASTeleportProjectile : public ARogueProjectileBase
{
	GENERATED_BODY()

public:

	ASTeleportProjectile();

protected:

	/* Re-usable handle for the initial explosion delay and the teleportation (only 1 timer must be active) */
	FTimerHandle TeleportHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float DetonateDelay = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float TeleportSecondaryDelay = 0.2f;

	void StartDelayedTeleport();

	void HandleTeleportation();

	virtual void BeginPlay() override;

	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) override;
};
