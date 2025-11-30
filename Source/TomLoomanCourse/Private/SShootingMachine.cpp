// Fill out your copyright notice in the Description page of Project Settings.


#include "SShootingMachine.h"

#include "SProjectileBase.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASShootingMachine::ASShootingMachine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow component");
}

// Called when the game starts or when spawned
void ASShootingMachine::BeginPlay()
{
	Super::BeginPlay();
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(Timer, this, &ASShootingMachine::SpawnProjectile, ProjectileSpawnRate, true);
	}	
}

void ASShootingMachine::SpawnProjectile()
{
	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC) return;
	
	const FVector StartLocation = GetActorLocation();
	FTransform SpawnTransform;
	
	FVector PlayerLocation;
	FRotator PlayerRotation;
	if (bShouldTrackPlayer &&  FindPlayer(PlayerLocation, PlayerRotation))
	{
		const FVector TargetPoint = PlayerLocation;
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetPoint);
		SpawnTransform = FTransform(SpawnRotation, StartLocation);
	}
	else
	{
		SpawnTransform = UKismetMathLibrary::MakeTransform(StartLocation, GetActorRotation());
	}
	
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	if (Projectile)
	{
		AActor* spawnedProjectile = World->SpawnActor<ASProjectileBase>(Projectile, SpawnTransform, SpawnParams);
	}
}

bool ASShootingMachine::FindPlayer(FVector& OutLocation, FRotator& OutDirection)
{
	UWorld* World = GetWorld();
	if (!World) return false;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC) return false;

	APawn* PlayerPawn = PC->GetPawn();
	if (!PlayerPawn) return false;

	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	
	FHitResult HitResult;
	if (!World->LineTraceSingleByChannel(HitResult, GetActorLocation(), PlayerLocation, ECC_Visibility))
	{
		DrawDebugLine(World, GetActorLocation(), PlayerLocation, FColor::Red, false, 1.0f, 0, 2.0f);
		OutLocation = PlayerLocation;
		OutDirection = PlayerPawn->GetActorRotation();
		return true;
	}
	return false;
}