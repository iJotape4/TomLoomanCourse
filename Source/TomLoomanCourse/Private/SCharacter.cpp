// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SAnimInstance.h"
#include "SInteractionComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NativeGameplayTags.h"
#include "TomLoomanCourse/SharedGameplayTags.h"
#include "TomLoomanCourse/ActionSystem/URogueActionSystemComponent.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Action_Sprint, "Action.Sprint");

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

	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>("AttributesComponent");
	
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

void ASCharacter::SwitchAction(const FInputActionValue& Value)
{
	SelectedAttackName = ActionSystemComponent->GetNextAttackName(SelectedAttackName);
	
}

void ASCharacter::StartAction(const FInputActionValue& Value)
{
	ActionSystemComponent->StartAction(SelectedAttackName);
}

void ASCharacter::StartAction(FGameplayTag InAction)
{
	ActionSystemComponent->StartAction(InAction);
}

void ASCharacter::StopAction(FGameplayTag InActionName)
{
	ActionSystemComponent->StopAction(InActionName);
}

void ASCharacter::HandleHealthChanged(AActor* InstigatorActor, URogueActionSystemComponent* OwningComp, float NewHealth, float Delta)
{
	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		CharacterMesh->SetColorParameterValueOnMaterials(Parameter_Color, Delta > 0.0f ? HealedColor : DamagedColor);
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
		if (ActionSystemComponent && AnimInstance)
		{
			ActionSystemComponent->OnDeath.AddDynamic(AnimInstance, &USAnimInstance::Death);
			ActionSystemComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::HandleHealthChanged);
		}
	}	
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (ensure(ActionSystemComponent))
	{
		ActionSystemComponent->OnDeath.AddDynamic(this, &ASCharacter::HandleOnPawnDeath);
		SelectedAttackName = ActionSystemComponent->GetNextAttackName(FGameplayTag::EmptyTag);
	}
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
		if (Input_Move)
		{
			EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		}

		if (Input_Look)
		{
			EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ASCharacter::Look);
		}
		
		if (Input_Jump)
		{
			EnhancedInput->BindAction(Input_Jump, ETriggerEvent::Started, this, &ASCharacter::JumpStarted);
			EnhancedInput->BindAction(Input_Jump, ETriggerEvent::Completed, this, &ASCharacter::JumpCompleted);
		}
		if (Input_Sprint)
		{
			EnhancedInput->BindAction(Input_Sprint, ETriggerEvent::Started, this, &ThisClass::StartAction, SharedGameplayTags::Action_Sprint.GetTag());
			EnhancedInput->BindAction(Input_Sprint, ETriggerEvent::Completed, this, &ThisClass::StopAction, SharedGameplayTags::Action_Sprint.GetTag());
		}

		if (Input_PrimaryAttack)
		{
			EnhancedInput->BindAction(Input_PrimaryAttack, ETriggerEvent::Started, this, &ASCharacter::StartAction);
		}

		if (Input_Interaction)
		{
			EnhancedInput->BindAction(Input_Interaction, ETriggerEvent::Started, this, &ASCharacter::PrimaryInteract);
		}

		if (Input_SwitchWeapon)
		{
			EnhancedInput->BindAction(Input_SwitchWeapon, ETriggerEvent::Started, this, &ASCharacter::SwitchAction);
		}
	}
}