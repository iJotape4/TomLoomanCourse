// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STargetDummy.generated.h"

class USAttributesComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASTargetDummy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASTargetDummy();

	UPROPERTY(VisibleAnywhere)
	USAttributesComponent* AttributesComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Config")
	FName Parameter_TimeToHit = FName("TimeToHit");

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class USDamagePopUp_Widget> DamagePopUpBlueprintClass;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
