// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"

#include "TomLoomanCourse/ActionSystem/URogueActionSystemComponent.h"
#include "SDamagePopUp_Widget.h"
#include "TomLoomanCourse/SharedGameplayTags.h"


// Sets default values
ASTargetDummy::ASTargetDummy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	RootComponent = Mesh;

	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>("Attributes Component");
	ActionSystemComponent->SetDefaultAttributeSet(URogueHealthAttributeSet::StaticClass());
	ActionSystemComponent->GetAttributeListener(SharedGameplayTags::Attribute_Health).AddUObject(this, &ThisClass::OnHealthChanged);
}

void ASTargetDummy::OnHealthChanged(FGameplayTag AttributesTag, float NewHealth, float OldHealth)
{
	if (UWorld* World = GetWorld())
	{
		Mesh->SetScalarParameterValueOnMaterials(Parameter_TimeToHit, World->TimeSeconds);

		if (IsValid(DamagePopUpBlueprintClass))
		{
			if (USDamagePopUp_Widget* DamagePopUpWidget = CreateWidget<USDamagePopUp_Widget>(World, DamagePopUpBlueprintClass))
			{
				DamagePopUpWidget->AttachTo = this;
				DamagePopUpWidget->DamageText = FMath::Abs(OldHealth - NewHealth);
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