// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class USRadialForceComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;
class USoundBase;
class UStaticMeshComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:

	ASExplosiveBarrel();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	USRadialForceComponent* RadialForceComponent;
	
	UPROPERTY()
	FVector ExplosionLocation =	FVector(0,0,120);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Config")
	float RadialForceDamage = 50.0f;

	UPROPERTY(EditAnywhere, Category= "Explosive Barrel")
	float ExplosionDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category= "Explosive Barrel")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category= "Explosive Barrel")
	TObjectPtr<USoundBase> ExplosionSound;

	UPROPERTY(EditDefaultsOnly, Category= "Explosive Barrel")
	TObjectPtr<UNiagaraSystem> BurningEffect;

	UPROPERTY(EditDefaultsOnly, Category= "Explosive Barrel")
	TObjectPtr<USoundBase> BurningSound;

	UFUNCTION(BlueprintCallable)
	void Explode();

	bool bExploded = false;

	FTimerHandle ExplosionTimerHandle;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> ActiveBurningEffectComp = nullptr;

	UPROPERTY()
	TObjectPtr<UAudioComponent> ActiveBurningSoundComp = nullptr;
};
