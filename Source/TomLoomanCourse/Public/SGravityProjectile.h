// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARogueProjectileBase.h"
#include "SGravityProjectile.generated.h"

class URadialForceComponent;

UCLASS(Abstract)
class TOMLOOMANCOURSE_API ASGravityProjectile : public ARogueProjectileBase
{
	GENERATED_BODY()

public:

	ASGravityProjectile();

	virtual void PostInitializeComponents() override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<URadialForceComponent> RadialForceComponent;

	UFUNCTION()
	void OnSphereOverlappedActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
