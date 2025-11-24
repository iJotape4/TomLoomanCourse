// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SInteractionComponent.h"
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

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	// Determine world target from center of screen
	APlayerController* PC = Cast<APlayerController>(GetController());
	FVector TargetPoint = FVector::ZeroVector;
	const float TraceDistance = 10000.f;

	if (PC && CameraComp)
	{
		int32 ViewportX = 0, ViewportY = 0;
		PC->GetViewportSize(ViewportX, ViewportY);
		FVector2D ScreenCenter(ViewportX * 0.5f, ViewportY * 0.5f);

		FVector WorldLocation, WorldDirection;
		if (PC->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection))
		{
			FVector TraceEnd = WorldLocation + (WorldDirection * TraceDistance);

			FHitResult Hit;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			QueryParams.bTraceComplex = true;

			// Prefer visibility channel for camera traces
			if (World->LineTraceSingleByChannel(Hit, WorldLocation, TraceEnd, ECC_Visibility, QueryParams))
			{
				TargetPoint = Hit.ImpactPoint;
			}
			else
			{
				TargetPoint = TraceEnd;
			}
		}
	}

	//TO DO: Delete this if unuseful
	// Fallback: use camera forward vector if deproject failed
	if (TargetPoint.IsZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("Camera Deprojection failed in %s:%d. Using camera forward vector as target."), ANSI_TO_TCHAR(__FILE__), __LINE__);
		if (CameraComp)
		{
			TargetPoint = CameraComp->GetComponentLocation() + (CameraComp->GetComponentRotation().Vector() * TraceDistance);
		}
		else
		{
			TargetPoint = HandLocation + (GetActorForwardVector() * TraceDistance);
		}
	}

	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TargetPoint);
	FTransform SpawnTransform(SpawnRotation, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	if (ProjectileClass)
	{
		World->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
	}
}

void ASCharacter::PrimaryInteract(const FInputActionValue& Value)
{
	InteractionComponent->PrimaryInteract();
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASCharacter::JumpStarted(const FInputActionValue& Value)
{
	Jump();
}

void ASCharacter::JumpCompleted(const FInputActionValue& Value)
{
	StopJumping();
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
	}
}