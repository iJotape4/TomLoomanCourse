// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "SProjectileBase.h"
#include "PhysicsEngine/RadialForceComponent.h"




// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	RootComponent = StaticMeshComponent;
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("Radial Force Component");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
	RadialForceComponent->bAutoActivate = false;
	RadialForceComponent->Radius = 700.0f;
	RadialForceComponent->ImpulseStrength = 1000.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnComponentHit);
}

void ASExplosiveBarrel::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ASProjectileBase* HitActor = Cast<ASProjectileBase>(OtherActor))
	{
		RadialForceComponent->FireImpulse();
	}
}