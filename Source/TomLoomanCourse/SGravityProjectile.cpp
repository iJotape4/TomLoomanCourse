// Fill out your copyright notice in the Description page of Project Settings.


#include "SGravityProjectile.h"

#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASGravityProjectile::ASGravityProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AffectedCollisionChannels.Add(ECC_WorldDynamic);
	AffectedCollisionChannels.Add(ECC_PhysicsBody);
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("Radial Force Comp");
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->Radius = 700.0f;
	RadialForceComponent->ForceStrength = -5000000.0f;
	RadialForceComponent->bImpulseVelChange = true;
	
	RadialForceComponent->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // Remove WorldStatic

	for (auto Element : AffectedCollisionChannels)
	{
		RadialForceComponent->AddCollisionChannelToAffect(Element);
	}
}

// Called when the game starts or when spawned
void ASGravityProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ASGravityProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnComponentHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	if (OtherComp)
	{
		ECollisionChannel HitChannel = OtherComp->GetCollisionObjectType();
		if (AffectedCollisionChannels.Contains(HitChannel) && OtherComp->IsSimulatingPhysics())
		{
			OtherActor->Destroy();
		}
	}	
}