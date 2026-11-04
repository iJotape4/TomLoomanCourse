// Fill out your copyright notice in the Description page of Project Settings.


#include "SGravityProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"


ASGravityProjectile::ASGravityProjectile()
{
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->ForceStrength = -800000.0f; // Negative to pull in instead of push out
	RadialForceComponent->Radius = 1200.0f;
	// Avoid sucking the player into the blackhole
	RadialForceComponent->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	// Suck up nearby objects, small enough to let them miss the sphere and flail around a bit first
	SphereComponent->SetSphereRadius(20.0f);
	// Profile only overlaps physics bodies and never blocks anything, so it passes through the world.
	// OnActorHit from the base class will therefore never trigger, as intended for this projectile.
	SphereComponent->SetCollisionProfileName("BlackholeCore");
	// Slow
	MovementComponent->InitialSpeed = 500.0f;

	LifeTime = 5.0f;
}

void ASGravityProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Note: Make sure GenerateOverlapEvents is enabled on the cubes in the world
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASGravityProjectile::OnSphereOverlappedActor);
}

void ASGravityProjectile::OnSphereOverlappedActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherActor->Destroy();
	}
}
