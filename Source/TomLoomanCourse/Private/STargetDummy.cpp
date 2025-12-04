// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"

#include "SAttributesComponent.h"
#include "SDamagePopUp_Widget.h"


// Sets default values
ASTargetDummy::ASTargetDummy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	RootComponent = Mesh;

	AttributesComponent = CreateDefaultSubobject<USAttributesComponent>("Attributes Component");
	AttributesComponent->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (UWorld* World = GetWorld())
	{
		Mesh->SetScalarParameterValueOnMaterials(Parameter_TimeToHit, World->TimeSeconds);

		if (IsValid(DamagePopUpBlueprintClass))
		{
			if (USDamagePopUp_Widget* DamagePopUpWidget = CreateWidget<USDamagePopUp_Widget>(World, DamagePopUpBlueprintClass))
			{
				DamagePopUpWidget->AttachTo = this;
				DamagePopUpWidget->DamageText = FMath::Abs(Delta);
				DamagePopUpWidget->Display();
			}
		}
	}
}

// Called when the game starts or when spawned
void ASTargetDummy::BeginPlay()
{
	Super::BeginPlay();
}	

// Called every frame
void ASTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}