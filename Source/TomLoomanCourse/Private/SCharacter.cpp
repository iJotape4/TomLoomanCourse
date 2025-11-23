// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

void ASCharacter::PrimaryAttack(const FInputActionValue& Value)
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FTransform Spawn = FTransform(GetControlRotation(), HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =  ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, Spawn, SpawnParams);
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