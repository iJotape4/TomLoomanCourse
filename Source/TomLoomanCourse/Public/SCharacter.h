// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SCharacter.generated.h"


class URogueAction;
class UNiagaraSystem;
class URogueActionSystemComponent;
class ARogueProjectileBase;
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
	UPROPERTY(VisibleAnywhere, Category= "Attack");
	int SelectedAttackIndex = 0;
	
	UPROPERTY(EditAnywhere, Category= "Attack");
	UAnimMontage* AnimAttack;
	FTimerHandle TimerHandle_PrimaryAttack;

	UPROPERTY(EditDefaultsOnly, Category= "Attack")
	float PrimaryAttackTraceDistance = 10000.f;

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
	URogueActionSystemComponent* ActionSystemComponent;

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
	FColor DamagedColor = FColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Config | Materials | Flash")
	FColor HealedColor = FColor::Green;

	// -------- Enhanced Input Actions --------

	/** Move input (Vector2D) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* Input_Move;

	/** Look input (Vector2D) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* Input_Look;

	/** Jump input (Digital) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* Input_Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* Input_Sprint;

	/** Fire Input(Digital) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* Input_PrimaryAttack;

	/** Switch Weapon Input(1D Axis) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* Input_SwitchWeapon;
	
	/** Interact Input (Digital) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Input")
	UInputAction* Input_Interaction;
	
	// -------- Input callbacks --------

	/** Called for forwards/backwards and right/left input */
	void Move(const FInputActionValue& Value);

	// /** Called for looking up/down and turning */
	void Look(const FInputActionValue& Value);
	
	void JumpStarted(const FInputActionValue& Value);
	void JumpCompleted(const FInputActionValue& Value);
	
	void PrimaryInteract(const FInputActionValue& Value);

	void SwitchAction(const FInputActionValue& Value);

	void StartAction(const FInputActionValue& Value);

	UFUNCTION()
	void HandleHealthChanged(AActor* InstigatorActor, URogueActionSystemComponent* OwningComp, float NewHealth, float Delta);
	UFUNCTION()
	void HandleOnPawnDeath(AActor* InstigatorActor);
	
	// Helpers
	FVector CalculateAimTargetPoint(float TraceDistance) const;
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