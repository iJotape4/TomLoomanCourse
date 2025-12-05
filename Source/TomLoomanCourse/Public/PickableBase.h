// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "PickableBase.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS(Abstract)
class TOMLOOMANCOURSE_API APickableBase : public AActor, public ISGameplayInterface
{
public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

private:
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Components")
	UNiagaraComponent* NiagaraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Config")
	UNiagaraSystem* PickUpEffect;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Config")
	bool bHasBeenCollected;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Config | Multi Pick")
	bool bMultiPickable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Config | Multi Pick",  meta = (EditCondition = "bMultiPickable", EditConditionHides))
	float RespawnTime = 10.0f;
	
	FTimerHandle TimerHandle;

	// Sets default values for this actor's properties
	APickableBase();

protected:
	void TriggerPickUpEffect();
	void RespawnOnTimeElapsed();
	void TogglePickableState(bool bIsPickable);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
