// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SMagicProjectile.generated.h"

UCLASS()
class TOMLOOMANCOURSE_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Config")
	UNiagaraSystem* EmitterOnCrash;
	
	virtual void PostInitializeComponents() override;
	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	ASMagicProjectile();
};
