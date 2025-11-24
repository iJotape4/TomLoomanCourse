// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"


// Sets default values
ASItemChest::ASItemChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	GoldPile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GoldPile"));
	GoldPile->SetupAttachment(BaseMesh);

	LidMeshTargetPitch = 110.0f;
}

void ASItemChest::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	FOnTimelineFloat ProgressUpdate;
	ProgressUpdate.BindUFunction(this, "OpeningUpdate");

	FOnTimelineEvent FinishedEvent;
	FinishedEvent.BindUFunction(this, "OpeningFinished");
	
	OpeningTimeline.AddInterpFloat(OpeningCurve, ProgressUpdate);
	OpeningTimeline.SetTimelineFinishedFunc(FinishedEvent);
}

void ASItemChest::OpeningUpdate(float Alpha)
{
	float NewPitch = FMath::Lerp(0, LidMeshTargetPitch, Alpha);
	LidMesh->SetRelativeRotation(FRotator(NewPitch, 0, 0));
}

void ASItemChest::OpeningFinished()
{
	bIsOpen = !bIsOpen;
}

void ASItemChest::StartOpen()
{
	!bIsOpen? OpeningTimeline.Play() : OpeningTimeline.Reverse();
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	StartOpen();	
}

void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OpeningTimeline.TickTimeline(DeltaTime);
}