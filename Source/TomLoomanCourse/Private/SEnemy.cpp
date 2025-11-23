// Fill out your copyright notice in the Description page of Project Settings.


#include "SEnemy.h"

#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "Runtime/AIModule/Classes/Perception/PawnSensingComponent.h"


// Sets default values
ASEnemy::ASEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensing Component");

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline Component");
}

// Called when the game starts or when spawned
void ASEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

