// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableBase.h"

#include "NiagaraComponent.h"


// Sets default values
APickableBase::APickableBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	RootComponent = MeshComp;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComp");
	NiagaraComponent->SetupAttachment(MeshComp);
}

void APickableBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if (bHasBeenCollected) return;
	
	if (PickUpEffect) TriggerPickUpEffect();
	
	if (bMultiPickable)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &APickableBase::RespawnOnTimeElapsed, RespawnTime);
		TogglePickableState(false);
	}
	else
	{
		Destroy();
	}
}

void APickableBase::TriggerPickUpEffect()
{
	NiagaraComponent->Activate(true);
}


void APickableBase::RespawnOnTimeElapsed()
{
	TogglePickableState(true);
}

void APickableBase::TogglePickableState(bool bIsPickable)
{
	bHasBeenCollected = !bIsPickable;
	MeshComp->Activate(bIsPickable);
}

// Called when the game starts or when spawned
void APickableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

