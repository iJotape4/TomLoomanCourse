// Fill out your copyright notice in the Description page of Project Settings.


#include "EmotionalTrigger.h"

#include "Components/BoxComponent.h"


// Sets default values
AEmotionalTrigger::AEmotionalTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	RootComponent = StaticMeshComponent;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionProfileName("Trigger");
}

void AEmotionalTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("overlapping"));
}

// Called when the game starts or when spawned
void AEmotionalTrigger::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEmotionalTrigger::OnComponentBeginOverlap);
}

// Called every frame
void AEmotionalTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

