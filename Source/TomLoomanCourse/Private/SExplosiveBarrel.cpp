// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "SMagicProjectile.h"
#include "PhysicsEngine/RadialForceComponent.h"


void ASExplosiveBarrel::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ASMagicProjectile* HitActor = Cast<ASMagicProjectile>(OtherActor))
	{
		RadialForceComponent->FireImpulse();
	}
}

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	StaticMeshComponent->SetNotifyRigidBodyCollision(true);
	RootComponent = StaticMeshComponent;
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("Radial Force Component");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
	RadialForceComponent->Radius = 700.0f;
	RadialForceComponent->ImpulseStrength = 1000.0f;
	RadialForceComponent->bImpulseVelChange = true;
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnComponentHit);
	
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

