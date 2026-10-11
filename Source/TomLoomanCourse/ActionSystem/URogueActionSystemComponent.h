// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RogueAttributeSet.h"
#include "Components/ActorComponent.h"
#include "URogueActionSystemComponent.generated.h"

struct FGameplayTag;
class URogueAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginPlay, URogueActionSystemComponent*, OwningComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, URogueActionSystemComponent*, OwningComp,  float, InHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, InstigatorActor);

UENUM()
enum EAttributeModifyType
{
	Base,
	Modifier, 
	OverrideBase,
	Invalid
};

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
	
	bool bIsAlive =true;
	
	UPROPERTY()
	TArray<TObjectPtr<URogueAction>> Actions;

	UPROPERTY(EditAnywhere, Category= "Actions")
	TArray<TSubclassOf<URogueAction>> DefaultActions;
	
	UPROPERTY()
	TObjectPtr<URogueAttributeSet> Attributes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", NoClear)
	TSubclassOf<URogueAttributeSet> AttributeSetClass;
	
	TMap<FGameplayTag, FRogueAttribute*> CatchedAttributes;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual  void InitializeComponent() override;

public:
	void StartAction(FGameplayTag InActionName);
	void StopAction(FGameplayTag InActionName);
	URogueAction* GrantAction(const TSubclassOf<URogueAction>& NewActionClass);
	
	UPROPERTY(Transient)
	FGameplayTagContainer ActiveGameplayTags;
	
	void ApplyAttributeChange(FGameplayTag InAttributeTag, float Delta, EAttributeModifyType ModifyType = Base);
	
	FRogueAttribute* GetAttribute(FGameplayTag InAttributeTag);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void Death(AActor* InstigatorActor);

	
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;	
	// Called every frame

	UFUNCTION(BlueprintCallable, Category= "Attributes")
	static URogueActionSystemComponent* GetAttributesComponent(AActor* FromActor);

	// Returns the attack after Current, wrapping around. Pass an empty tag to get the first attack.
	FGameplayTag GetNextAttackName(FGameplayTag Current) const;
};