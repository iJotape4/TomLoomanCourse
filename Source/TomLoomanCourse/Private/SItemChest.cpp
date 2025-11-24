// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"

#include "NiagaraComponent.h"


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

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("ParticleComp");
	NiagaraComponent->SetupAttachment(GoldPile);
	NiagaraComponent->bAutoActivate = false;
	
	LidOpenTargetPitch = 110.0f;
}

void ASItemChest::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	FOnTimelineFloat ProgressUpdate;
	ProgressUpdate.BindDynamic(this, &ASItemChest::LidOpeningUpdate);

	FOnTimelineEvent FinishedEvent;
	FinishedEvent.BindDynamic(this, &ASItemChest::LidOpeningFinished);
	
	LidOpeningTimeline.AddInterpFloat(LidOpeningCurve, ProgressUpdate);
	LidOpeningTimeline.SetTimelineFinishedFunc(FinishedEvent);
}

void ASItemChest::LidOpeningUpdate(const float Alpha)
{
	float NewPitch = FMath::Lerp(0, LidOpenTargetPitch, Alpha);
	LidMesh->SetRelativeRotation(FRotator(NewPitch, 0, 0));
}

void ASItemChest::LidOpeningFinished()
{
	bIsOpen = !bIsOpen;
	if (bIsOpen)
		NiagaraComponent->Activate();
}

void ASItemChest::ToggleOpen()
{
	!bIsOpen? LidOpeningTimeline.Play() : LidOpeningTimeline.Reverse();
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	ToggleOpen();	
}

void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LidOpeningTimeline.TickTimeline(DeltaTime);
}