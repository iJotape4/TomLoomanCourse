// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueAttributeSet.h"
#include "Components/ActorComponent.h"
#include "URogueActionSystemComponent.generated.h"

class URogueAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginPlay, URogueActionSystemComponent*, OwningComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, URogueActionSystemComponent*, OwningComp,  float, InHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, InstigatorActor);
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOMLOOMANCOURSE_API URogueActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URogueActionSystemComponent();

	UPROPERTY(BlueprintAssignable)
	FOnBeginPlay OnBeginPlay;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = "Attributes")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = "Attributes")
	float MaxHealth = 100.0f;

	bool bIsAlive =true;

	 UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FRogueAttribute Attributes;

	UPROPERTY()
	TArray<TObjectPtr<URogueAction>> Actions;

	UPROPERTY(EditAnywhere, Category= "Actions")
	TArray<TSubclassOf<URogueAction>> DefaultActions;
	
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual  void InitializeComponent() override;

public:
	void StartAction(FName InActionName);
	void StartAction(const TSubclassOf<URogueAction>& InActionClass);
	void StartAction(int ActionIndex);
	void GrantAction(const TSubclassOf<URogueAction>& NewActionClass);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta);

	bool IsFullHealth() const;
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void Death();

	
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;	
	// Called every frame

	UFUNCTION(BlueprintCallable, Category= "Attributes")
	static URogueActionSystemComponent* GetAttributesComponent(AActor* FromActor);

	int GetActionsCount() const {return Actions.Num();};
};