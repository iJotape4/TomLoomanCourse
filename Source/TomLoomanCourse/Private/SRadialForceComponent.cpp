// Fill out your copyright notice in the Description page of Project Settings.


#include "SRadialForceComponent.h"

#include "SAttributesComponent.h"
#include "Engine/OverlapResult.h"


// Sets default values for this component's properties
USRadialForceComponent::USRadialForceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

TArray<AActor*, TInlineAllocator<1>> USRadialForceComponent::MakeExplosion()
{
	FVector Origin = GetOwner()->GetActorLocation();
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(FireImpulseOverlap),  false);

	GetWorld()->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, CollisionObjectQueryParams, FCollisionShape::MakeSphere(Radius), Params);
		
	TArray<AActor*, TInlineAllocator<1>> AffectedActors;
	AffectedActors.Reserve(Overlaps.Num());
		
	for(FOverlapResult& OverlapResult : Overlaps)
	{
		if(UPrimitiveComponent* PrimitiveComponent = OverlapResult.Component.Get())
		{
			AffectedActors.AddUnique(PrimitiveComponent->GetOwner());
		}
	}
	
	for(AActor* AffectedActor : AffectedActors)
	{
		if(DestructibleDamage > UE_SMALL_NUMBER)
		{
			if(USAttributesComponent* DestructibleInstance = Cast<USAttributesComponent>(AffectedActor->GetComponentByClass(USAttributesComponent::StaticClass())))
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *AffectedActor->GetName());
				DestructibleInstance->ApplyHealthChange(-DestructibleDamage);
			}
		}
	}
	FireImpulse();
	
	return AffectedActors;
}


// Called when the game starts
void USRadialForceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USRadialForceComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

