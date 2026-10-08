// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction_ProjectileAttack.h"

#include "ARogueProjectileBase.h"
#include "NiagaraFunctionLibrary.h"
#include "RogueGameTypes.h"
#include "URogueActionSystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

TAutoConsoleVariable<float> CvarProjectileAdjustmentDebugDrawing(TEXT("game.projectile.DebugDraw"),
	0.f, TEXT("Enable projectile aim adjustment debug rendering. ( 0 = off, > 0 is duration"),
	ECVF_Cheat);

URogueAction_ProjectileAttack::URogueAction_ProjectileAttack()
{
	MuzzleSocketName = TEXT("Muzzle_01");
	CooldownTime = 0.5f;
}

void URogueAction_ProjectileAttack::PostInitProperties()
{
	Super::PostInitProperties();
	const URogueActionSystemComponent* ActionComp = GetOwningComponent();
	if (ensure(!ActionComp)) return;
	
	Character = CastChecked<ACharacter>(ActionComp->GetOwner());
}

void URogueAction_ProjectileAttack::StartAction_Implementation()
{
	if (!ensure(Character)) return;
	
	Super::StartAction_Implementation();
	
	Character->PlayAnimMontage(AttackMontage);
	UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, Character->GetMesh(), MuzzleSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	UGameplayStatics::PlaySound2D(this, CastingSound);

	FTimerHandle AttacktimerHandle;
	const float AttackDelayTime = 0.2f;

	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &ThisClass::AttackTimerElapsed);
	GetWorld()->GetTimerManager().SetTimer(AttacktimerHandle, this, &ThisClass::AttackTimerElapsed, AttackDelayTime, false);
}

void URogueAction_ProjectileAttack::AttackTimerElapsed()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(MuzzleSocketName);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Character;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector EyeLocation;
	FRotator EyeRotation;

	Character->GetController()->GetPlayerViewPoint(EyeLocation, EyeRotation);

	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * MaxRange);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);

	FVector(AdjustTargetLocation);
	FHitResult Hit;

	if (World->LineTraceSingleByChannel(Hit, SpawnLocation, TraceEnd, COLLISION_PROJECTILE, QueryParams))
		AdjustTargetLocation = Hit.Location;
	else
		AdjustTargetLocation = TraceEnd;

	FRotator SpawnRotation = (AdjustTargetLocation - SpawnLocation).Rotation();
	AActor* Newprojectile = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	Character->MoveIgnoreActorAdd(Newprojectile);
	StopAction();

#if !UE_BUILD_SHIPPING
	float DebugDrawDuration = CvarProjectileAdjustmentDebugDrawing.GetValueOnGameThread();
	if (DebugDrawDuration > 0.f)
	{
		DrawDebugBox(World, AdjustTargetLocation, FVector(20.0f), FColor::Green, false, DebugDrawDuration);
		DrawDebugLine(World, EyeLocation, TraceEnd, FColor::Green, false, DebugDrawDuration);
		DrawDebugLine(World, SpawnLocation, AdjustTargetLocation, FColor::Yellow, false, DebugDrawDuration);
		DrawDebugLine(World, SpawnLocation, SpawnLocation+(EyeRotation.Vector() * 5000.0f), FColor::Purple, false, DebugDrawDuration);
		
	}
#endif
}

FVector URogueAction_ProjectileAttack::GetHandLocation() const
{
	if (USkeletalMeshComponent* MeshComp = Character->GetMesh())
	{
		if (MeshComp->DoesSocketExist(MuzzleSocketName))
		{
			return MeshComp->GetSocketLocation(MuzzleSocketName);
		}
	}
	return Character->GetActorLocation();
}