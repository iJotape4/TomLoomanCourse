// Fill out your copyright notice in the Description page of Project Settings.


#include "SGravityProjectile.h"

#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ASGravityProjectile::ASGravityProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("Radial Force Comp");
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->Radius = 700.0f;
	RadialForceComponent->ForceStrength = -1000000.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // Remove WorldStatic
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void ASGravityProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASGravityProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

