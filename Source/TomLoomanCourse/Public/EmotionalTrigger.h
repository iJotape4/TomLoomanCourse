// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EmotionalTrigger.generated.h"

class UBoxComponent;

UCLASS()
class TOMLOOMANCOURSE_API AEmotionalTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEmotionalTrigger();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;


	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
