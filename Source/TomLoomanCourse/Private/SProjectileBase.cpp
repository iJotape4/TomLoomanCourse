// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "SCameraShake.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


// Sets default values
ASProjectileBase::ASProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComponent->SetCollisionProfileName("Projectile");
	RootComponent = SphereComponent;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("ParticleComp");
	NiagaraComponent->SetupAttachment(RootComponent);
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComp");
	MovementComponent->InitialSpeed = 1000.f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;
	MovementComponent->ProjectileGravityScale = 0.f;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(SphereComponent);
}


// Called when the game starts or when spawned
void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);
	AudioComponent->SetSound(FlightSFX);
	AudioComponent->Play();
}

void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ASProjectileBase::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ensure(IsPendingKillPending())) return;
	
	SpawnEmitter(Hit.Location);
	
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(World,ImpactSFX, Hit.Location);
		
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(World, 0);
		CameraManager->PlayWorldCameraShake(
			World,
			USCameraShake::StaticClass(), // The class of your custom camera shake
			GetActorLocation(),           // Epicenter of the shake
			0.0f,                         // InnerRadius
			1000.0f,                      // OuterRadius
			1.0f,                         // Falloff
			false                         // bOrientShakeTowardsEpicenter
		);
	}
}

void ASProjectileBase::SpawnEmitter(FVector Location)
{
	if (!EmitterOnCrash) return;
	UNiagaraComponent* NiagaraEmitter = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			EmitterOnCrash,
			Location,
			FRotator::ZeroRotator,
			FVector::One(), // Scale
			true,  // AutoDestroy
			true //AutoActivate
		);

}
