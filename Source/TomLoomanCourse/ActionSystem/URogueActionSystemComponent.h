// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RogueAttributeSet.h"
#include "Components/ActorComponent.h"
#include "URogueActionSystemComponent.generated.h"

struct FGameplayTag;
class URogueAction;

UENUM(BlueprintType)
enum EAttributeModifyType
{
	Base,
	Modifier, 
	OverrideBase,
	Multiplier,
	RemoveMultiplier,
	Invalid
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginPlay, URogueActionSystemComponent*, OwningComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, InstigatorActor);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged,FGameplayTag /*AttributeTag*/, float /*NewAttributeValue*/ , float /*OldAttributeValue*/);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnAttributeDynamicChanged,FGameplayTag , AttributeTag, float , NewAttributeValue , float ,	OldAttributeValue);


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
	FOnDeath OnDeath;

protected:
	
	bool bIsAlive =true;
	
	UPROPERTY()
	TArray<TObjectPtr<URogueAction>> Actions;

	UPROPERTY(EditAnywhere, Category= "Actions")
	TArray<TSubclassOf<URogueAction>> DefaultActions;
	
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<URogueAttributeSet> Attributes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", NoClear)
	TSubclassOf<URogueAttributeSet> AttributeSetClass;
	
	TMap<FGameplayTag, FRogueAttribute*> CachedAttributes;
	
	TMap<FGameplayTag, FOnAttributeChanged> AttributeListeners;
	TMap<FGameplayTag, TArray<FOnAttributeDynamicChanged>> AttributeDynamicListeners;
	
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
	
	UFUNCTION(BlueprintCallable)
	void ApplyAttributeChange(FGameplayTag InAttributeTag, float Delta, EAttributeModifyType ModifyType = Base);
	
	FRogueAttribute* GetAttribute(FGameplayTag InAttributeTag) const;
	
	UFUNCTION(BlueprintCallable)
	float GetAttributeValue(FGameplayTag InAttributeTag) const;
	
	FOnAttributeChanged& GetAttributeListener(FGameplayTag InAttributeTag);
	
	UFUNCTION(BlueprintCallable, DisplayName=" Add Attribute Listener", meta=(Keywords="events,delegate"))
	void AddAttributeDynamicListener(FOnAttributeDynamicChanged InDelegate, FGameplayTag InAttributeTag );

	UFUNCTION(BlueprintCallable, DisplayName=" Remove Attribute Listener", meta=(Keywords="events,delegate"))
	void RemoveAttributeDynamicListener(FOnAttributeDynamicChanged InDelegate );

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