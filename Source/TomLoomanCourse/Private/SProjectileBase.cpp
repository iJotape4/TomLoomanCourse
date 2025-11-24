// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
ASProjectileBase::ASProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComponent->SetCollisionProfileName("Projectile");
	RootComponent = SphereComponent;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("ParticleComp");
	NiagaraComponent->SetupAttachment(RootComponent);
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComp");
	MovementComponent->InitialSpeed = 1000.f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;
	MovementComponent->ProjectileGravityScale = 0.f;
}


// Called when the game starts or when spawned
void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);
}

void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnComponentHit);
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}
//
// void ASProjectileBase::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
// {
// 	UNiagaraComponent* NiagaraEmitter = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
// 		GetWorld(),
// 		EmitterOnCrash,
// 		Hit.Location,
// 		FRotator::ZeroRotator,
// 		FVector::One(), // Scale
// 		true,  // AutoDestroy
// 		true //AutoActivate
// 	);
// 	
// 	Destroy();
// }


