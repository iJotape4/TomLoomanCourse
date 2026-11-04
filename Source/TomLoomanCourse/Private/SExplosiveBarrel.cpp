// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "SRadialForceComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetSimulatePhysics(true);
	// Blueprint automatically sets this collision profile when enabling simulate physics, in C++ we do this manually.
	StaticMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	StaticMeshComponent->SetCanEverAffectNavigation(false);
	RootComponent = StaticMeshComponent;

	RadialForceComponent = CreateDefaultSubobject<USRadialForceComponent>("Radial Force Component");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
	// We don't need to Activate this component, that is only good for applying a constant radial force and not our desired one frame Impulse
	RadialForceComponent->bAutoActivate = false;
	RadialForceComponent->bIgnoreOwningActor = true;
	RadialForceComponent->Radius = 700.0f;
	RadialForceComponent->ImpulseStrength = 1000.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
	RadialForceComponent->DestructibleDamage = RadialForceDamage;
}

float ASExplosiveBarrel::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// Allow exploding once, also skip when timer is already running
	if (bExploded || GetWorldTimerManager().TimerExists(ExplosionTimerHandle))
	{
		return ActualDamage;
	}
	
	// Hold onto both the Niagara and Audio Components to disable them during the Explode()
	ActiveBurningEffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(BurningEffect, StaticMeshComponent, NAME_None,
		ExplosionLocation, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);

	ActiveBurningSoundComp = UGameplayStatics::SpawnSoundAttached(BurningSound, StaticMeshComponent);

	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ASExplosiveBarrel::Explode, ExplosionDelay);

	return ActualDamage;
}

void ASExplosiveBarrel::Explode()
{
	bExploded = true;

	if (ActiveBurningEffectComp)
	{
		ActiveBurningEffectComp->Deactivate();
	}
	if (ActiveBurningSoundComp)
	{
		ActiveBurningSoundComp->Stop();
	}

	// Tom's version calls FireImpulse() here; MakeExplosion() also fires the impulse
	// and additionally applies RadialForceDamage to actors with an action system component
	RadialForceComponent->MakeExplosion();

	StaticMeshComponent->AddImpulse(FVector::UpVector * 1000, NAME_None, true);
	StaticMeshComponent->AddAngularImpulseInDegrees(FVector::RightVector * 1000, NAME_None, true);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation()+ExplosionLocation, GetActorRotation());

	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
}
