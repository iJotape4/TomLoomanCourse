// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentHit.AddDynamic(this, &ASMagicProjectile::OnComponentHit);
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ASMagicProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UNiagaraComponent* NiagaraEmitter = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		EmitterOnCrash,
		Hit.Location,
		FRotator::ZeroRotator,
		FVector::One(), // Scale
		true,  // AutoDestroy
		true //AutoActivate
	);
	
	Destroy();
}
