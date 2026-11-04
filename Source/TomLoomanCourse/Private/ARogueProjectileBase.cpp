// Fill out your copyright notice in the Description page of Project Settings.


#include "ARogueProjectileBase.h"

#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SCameraShake.h"


ARogueProjectileBase::ARogueProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComponent->SetCollisionProfileName("Projectile");
	RootComponent = SphereComponent;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleComp"));
	NiagaraComponent->SetupAttachment(SphereComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(SphereComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	MovementComponent->InitialSpeed = 1000.f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;
	MovementComponent->ProjectileGravityScale = 0.f;
}

void ARogueProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentHit.AddDynamic(this, &ARogueProjectileBase::OnActorHit);
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ARogueProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeTime);

	if (FlightSFX)
	{
		AudioComponent->SetSound(FlightSFX);
		AudioComponent->Play();
	}
}

void ARogueProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	PlayExplodeEffects();

	Destroy();
}

void ARogueProjectileBase::PlayExplodeEffects()
{
	const FVector Location = GetActorLocation();

	if (EmitterOnCrash)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, EmitterOnCrash, Location);
	}

	if (ImpactSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSFX, Location, FRotator::ZeroRotator);
	}

	UGameplayStatics::PlayWorldCameraShake(this, USCameraShake::StaticClass(), Location,
		0.0f,     // InnerRadius
		1000.0f,  // OuterRadius
		1.0f,     // Falloff
		false);   // bOrientShakeTowardsEpicenter
}
