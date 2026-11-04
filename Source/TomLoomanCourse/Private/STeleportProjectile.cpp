// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"


ASTeleportProjectile::ASTeleportProjectile()
{
	MovementComponent->InitialSpeed = 6000.0f;
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TeleportHandle, this, &ThisClass::StartDelayedTeleport, DetonateDelay);
}

void ASTeleportProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Skip the base implementation, we must delay destroying and avoid playing duplicate explosion effects
	//Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	// Cancel the timer to prevent a second teleportation
	GetWorldTimerManager().ClearTimer(TeleportHandle);

	StartDelayedTeleport();
}

void ASTeleportProjectile::StartDelayedTeleport()
{
	PlayExplodeEffects();

	// Prevent moving further ahead while we wait for the teleport to trigger
	MovementComponent->StopMovementImmediately();
	// Hide all visuals and prevent any further collision while we wait on the teleport timer
	NiagaraComponent->Deactivate();
	AudioComponent->Stop();
	SetActorEnableCollision(false);

	// Another delay until we teleport (so the player has a chance to see the VFX before we move them)
	GetWorldTimerManager().SetTimer(TeleportHandle, this, &ThisClass::HandleTeleportation, TeleportSecondaryDelay);
}

void ASTeleportProjectile::HandleTeleportation()
{
	if (APawn* ActorToTeleport = GetInstigator())
	{
		// Handles de-penetration on nearby collision geometry
		// note: the teleport call might fail if it cannot find any valid location
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation());
	}

	// Can't destroy any sooner, that would prevent the timers from running on a valid Actor
	Destroy();
}
