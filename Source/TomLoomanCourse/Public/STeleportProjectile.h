// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "STeleportProjectile.generated.h"

UCLASS()
class TOMLOOMANCOURSE_API ASTeleportProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category = "Config")
	float TravelTime =0.2f;
	
	UPROPERTY(EditAnywhere, Category = "Config")
	float TimeToActivateEffect = 0.2f;
	FTimerHandle TimerHandle_Teleport;
	
	// Sets default values for this actor's properties
	ASTeleportProjectile();
	virtual  void PostInitializeComponents() override;

	virtual void OnTimeElapasedAfterSpawn() override;
	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	void Teleport();
};
