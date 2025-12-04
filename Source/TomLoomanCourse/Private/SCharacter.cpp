// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraFunctionLibrary.h"
#include "SAnimInstance.h"
#include "SAttributesComponent.h"
#include "SInteractionComponent.h"
#include "SProjectileBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = true;

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("InteractionComponent");

	AttributesComponent = CreateDefaultSubobject<USAttributesComponent>("AttributesComponent");
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	
}

void ASCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		//Tom Looman defined it like:
		//const FVector ForwardDirection = YawRotation.Vector();
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);
	
		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASCharacter::JumpStarted(const FInputActionValue& Value)
{
	Jump();
}

void ASCharacter::JumpCompleted(const FInputActionValue& Value)
{
	StopJumping();
}

void ASCharacter::PrimaryAttack(const FInputActionValue& Value)
{
	PlayAnimMontage(AnimAttack);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	UWorld* World = GetWorld();
	if (!World) return;

	const FVector HandLocation = GetHandLocation();
	const FVector TargetPoint = CalculateAimTargetPoint(PrimaryAttackTraceDistance);

	const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TargetPoint);
	const FTransform SpawnTransform(SpawnRotation, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	if (ensure(CurrentProjectile))
	{
		AActor* Projectile = World->SpawnActor<ASProjectileBase>(CurrentProjectile, SpawnTransform, SpawnParams);
		GetCapsuleComponent()->IgnoreActorWhenMoving(Projectile, true);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),MuzzleFlashVFX, GetHandLocation(), FRotator::ZeroRotator);
	}
}

FVector ASCharacter::GetHandLocation() const
{
	const FName HandSocketName(TEXT("Muzzle_01"));
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (MeshComp->DoesSocketExist(HandSocketName))
		{
			return MeshComp->GetSocketLocation(HandSocketName);
		}
	}
	return GetActorLocation();
}

FVector ASCharacter::CalculateAimTargetPoint(float TraceDistance) const
{
	const UWorld* World = GetWorld();
	if (!World) return FVector::ZeroVector;

	const APlayerController* PC = Cast<APlayerController>(GetController());

	// Start and Direction defaults
	FVector Start = GetActorLocation();
	FVector Direction = GetActorForwardVector();

	// Prefer the real camera view if available
	if (PC)
	{
		FVector CamLoc;
		FRotator CamRot;
		PC->GetPlayerViewPoint(CamLoc, CamRot);
		Start = CamLoc;
		Direction = CamRot.Vector();
	}
	else if (CameraComp)
	{
		Start = CameraComp->GetComponentLocation();
		Direction = CameraComp->GetComponentRotation().Vector();
	}

	const FVector End = Start + (Direction * TraceDistance);

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;

	if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams))
	{
		return Hit.ImpactPoint;
	}
	return End;
}

void ASCharacter::SwitchProjectile(const FInputActionValue& Value)
{
	if (ensure( Projectiles.Num() > 0))
	{
		Projectiles.Num() > 0 ? CurrentProjectileIndex = (CurrentProjectileIndex + 1) % Projectiles.Num() : CurrentProjectileIndex = 0;
		CurrentProjectile = Projectiles[CurrentProjectileIndex];
	}
}

void ASCharacter::HandleHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		CharacterMesh->SetColorParameterValueOnMaterials(Parameter_Color, Color);
		CharacterMesh->SetScalarParameterValueOnMaterials(Parameter_HitFlashSpeed, Speed);
		CharacterMesh->SetScalarParameterValueOnMaterials(Parameter_TimeToHit, GetWorld()->TimeSeconds);
	}
}

void ASCharacter::PrimaryInteract(const FInputActionValue& Value)
{
	InteractionComponent->PrimaryInteract();
}

void ASCharacter::HandleOnPawnDeath(AActor* InstigatorActor)
{
	UCapsuleComponent* CollisionComponent =  GetCapsuleComponent();
	CollisionComponent->SetLinearDamping(100.0f);
	CollisionComponent->SetAngularDamping(100.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	CollisionComponent->SetSimulatePhysics(true);
}
// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		USAnimInstance* AnimInstance =  Cast<USAnimInstance>(MeshComp->GetAnimInstance());
		if (AttributesComponent && AnimInstance)
		{
			AttributesComponent->OnDeath.AddDynamic(AnimInstance, &USAnimInstance::Death);
			AttributesComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::HandleHealthChanged);
		}
	}	
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (ensure( Projectiles.Num() > 0))
		CurrentProjectile = Projectiles[CurrentProjectileIndex];

	if (ensure(AttributesComponent))
		AttributesComponent->OnDeath.AddDynamic(this, &ASCharacter::HandleOnPawnDeath);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Ensure InputActions are assigned in the Blueprint or defaults
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		}

		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASCharacter::Look);
		}
		
		if (JumpAction)
		{
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ASCharacter::JumpStarted);
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASCharacter::JumpCompleted);
		}

		if (PrimaryAttackAction)
		{
			EnhancedInput->BindAction(PrimaryAttackAction, ETriggerEvent::Started, this, &ASCharacter::PrimaryAttack);
		}

		if (PrimaryInteractAction)
		{
			EnhancedInput->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &ASCharacter::PrimaryInteract);
		}

		if (SwitchWeaponAction)
		{
			EnhancedInput->BindAction(SwitchWeaponAction, ETriggerEvent::Started, this, &ASCharacter::SwitchProjectile);
		}
	}
}