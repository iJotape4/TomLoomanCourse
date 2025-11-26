// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SGravityProjectile.generated.h"

class URadialForceComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASGravityProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASGravityProjectile();

protected:

	UPROPERTY(VisibleAnywhere, Category= "Components")
	URadialForceComponent* RadialForceComponent;

	UPROPERTY(VisibleAnywhere, Category= "Config")
	TArray<TEnumAsByte<ECollisionChannel>> AffectedCollisionChannels;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void PostInitializeComponents() override;
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult) override;
};
