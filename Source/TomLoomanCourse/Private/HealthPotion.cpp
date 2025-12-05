// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotion.h"

#include "Components/SphereComponent.h"


// Sets default values
AHealthPotion::AHealthPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	SphereComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bDrawDebugCollisionSphere) return;
	DrawDebugSphere(GetWorld(), SphereComponent->GetComponentLocation(), SphereComponent->GetUnscaledSphereRadius(), 32, FColor::Red, false, 2.0f);
}