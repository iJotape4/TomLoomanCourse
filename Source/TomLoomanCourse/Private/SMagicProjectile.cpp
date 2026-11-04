// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TomLoomanCourse/ActionSystem/URogueActionSystemComponent.h"
#include "TomLoomanCourse/SharedGameplayTags.h"


ASMagicProjectile::ASMagicProjectile()
{
	MovementComponent->InitialSpeed = 2000.0f;

	DmgTypeClass = UDamageType::StaticClass();
}

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Keep the base implementation (explode effects + destroy)
	Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	const FVector HitFromDirection = GetActorRotation().Vector();

	UGameplayStatics::ApplyPointDamage(OtherActor, Damage, HitFromDirection, Hit, GetInstigatorController(),
		this, DmgTypeClass);
}
