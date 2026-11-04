// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARogueProjectileBase.h"
#include "SMagicProjectile.generated.h"

UCLASS(Abstract)
class TOMLOOMANCOURSE_API ASMagicProjectile : public ARogueProjectileBase
{
	GENERATED_BODY()

public:

	ASMagicProjectile();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Attributes")
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Attributes")
	TSubclassOf<UDamageType> DmgTypeClass;

	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) override;
};
