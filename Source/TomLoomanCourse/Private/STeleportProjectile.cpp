// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "Components/SphereComponent.h"


// Sets default values
ASTeleportProjectile::ASTeleportProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASTeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentHit.AddDynamic(this, &ASTeleportProjectile::OnComponentHit);
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASTeleportProjectile::OnTimeElapasedAfterSpawn, TimeToActivateEffect);
}

void ASTeleportProjectile::OnTimeElapasedAfterSpawn()
{
	Super::OnTimeElapasedAfterSpawn();
	GetWorldTimerManager().ClearTimer(TimerHandle_Teleport);
	Teleport();
}

void ASTeleportProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnComponentHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	Teleport();
}

void ASTeleportProjectile::Teleport()
{
	if (APawn* _Instigator = GetInstigator())
	{
		FVector ProjectileLocation = GetActorLocation();
		_Instigator->TeleportTo(ProjectileLocation, _Instigator->GetActorRotation());
	}
	Destroy();
}