// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SRadialForceComponent.generated.h"


class USAttributesComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOMLOOMANCOURSE_API USRadialForceComponent : public URadialForceComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USRadialForceComponent();

	TArray<AActor*, TInlineAllocator<1>> MakeExplosion();

	TArray<TEnumAsByte<enum EObjectTypeQuery> > GetObjectTypesToAffect() const { return ObjectTypesToAffect; };
	FCollisionObjectQueryParams GetCollisionObjectQueryParams() const { return CollisionObjectQueryParams; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};