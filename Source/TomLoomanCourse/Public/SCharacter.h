// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SCharacter.generated.h"


class UNiagaraSystem;
class USAttributesComponent;
class ASProjectileBase;
class USInteractionComponent;
class UInputMappingContext;
class UInputAction;

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category= "Attack")
	TArray<TSubclassOf<ASProjectileBase>> Projectiles;
	
	UPROPERTY(VisibleAnywhere, Category= "Attack");
	TSubclassOf<ASProjectileBase> CurrentProjectile;

	UPROPERTY(VisibleAnywhere, Category= "Attack");
	int CurrentProjectileIndex = 0;
	
	UPROPERTY(EditAnywhere, Category= "Attack");
	UAnimMontage* AnimAttack;
	FTimerHandle TimerHandle_PrimaryAttack;

	UPROPERTY(EditDefaultsOnly, Category= "Attack")
	float PrimaryAttackTraceDistance = 10000.f;

	UPROPERTY(EditAnywhere, Category= "Attack")
	UNiagaraSystem* MuzzleFlashVFX;

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere)	
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere)	
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USAttributesComponent* AttributesComponent;

	/// ---------- Other configs -------------- ////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Config | Materials | Flash")
	FName Parameter_TimeToHit = "TimeToHit";
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Config | Materials | Flash")
	FName Parameter_HitFlashSpeed = "HitFlashSpeed";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Config | Materials | Flash")
	float Speed = 4.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Config | Materials | Flash")
	FName Parameter_Color = "HitFlashColor";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Config | Materials | Flash")
	FColor Color = FColor::Red;
	
	// -------- Enhanced Input Actions --------

	/** Move input (Vector2D) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	/** Look input (Vector2D) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	/** Jump input (Digital) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	/** Fire Input(Digital) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* PrimaryAttackAction;

	/** Switch Weapon Input(1D Axis) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* SwitchWeaponAction;
	
	/** Interact Input (Digital) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Input")
	UInputAction* PrimaryInteractAction;
	
	// -------- Input callbacks --------

	/** Called for forwards/backwards and right/left input */
	void Move(const FInputActionValue& Value);

	// /** Called for looking up/down and turning */
	void Look(const FInputActionValue& Value);
	
	void JumpStarted(const FInputActionValue& Value);
	void JumpCompleted(const FInputActionValue& Value);

	void PrimaryAttack(const FInputActionValue& Value);
	void PrimaryAttack_TimeElapsed();

	void PrimaryInteract(const FInputActionValue& Value);

	void SwitchProjectile(const FInputActionValue& Value);

	UFUNCTION()
	void HandleHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);
	UFUNCTION()
	void HandleOnPawnDeath(AActor* InstigatorActor);
	
	// Helpers
	FVector CalculateAimTargetPoint(float TraceDistance) const;
	FVector GetHandLocation() const;
;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual  void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};