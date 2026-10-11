// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STargetDummy.generated.h"

struct FGameplayTag;
class URogueActionSystemComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASTargetDummy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASTargetDummy();

	UPROPERTY(VisibleAnywhere)
	URogueActionSystemComponent* ActionSystemComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Config")
	FName Parameter_TimeToHit = FName("TimeToHit");

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class USDamagePopUp_Widget> DamagePopUpBlueprintClass;

	UFUNCTION()
	void OnHealthChanged(FGameplayTag AttributesTag, float NewHealth, float OldHealth);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
