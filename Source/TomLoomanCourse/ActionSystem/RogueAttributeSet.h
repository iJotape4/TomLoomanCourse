// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "RogueAttributeSet.generated.h"

/**
 * 
 */

class URogueActionSystemComponent;

USTRUCT(BlueprintType)
struct FRogueAttribute
{
	GENERATED_BODY()

	FRogueAttribute() {}

	FRogueAttribute(float InBase, float InModifier = 0.0f, float InMultiplier = 1.0f) : 
	Base(InBase) , Modifier(InModifier) , Multiplier(InMultiplier) {}

	/* The base value, such as 'Strength' that was granted by the RPG class you picked, and modified permanently during gameplay (eg. +1 Str for a Level Up or a +1 permanent boost by consuming an item). This would be 'saved to disk'. */
	UPROPERTY(EditAnywhere)
	float Base = 0.0f;

	/* Temporary modifier from buffs/debuffs, equipped items. This would not be 'saved to disk' as items would re-apply themselves on load */
	UPROPERTY(Transient)
	float Modifier = 0.0f;
	
	UPROPERTY(Transient)
	float Multiplier = 1.0f;

	/* All game logic should get the value through here */
	
	float GetValue() const
	{
		// always clamp public value to zero, you could opt to make this a bool per attribute
		return FMath::Max( (Base + Modifier)*Multiplier, 0.0f);
	}
};

UCLASS()
class TOMLOOMANCOURSE_API URogueAttributeSet : public UObject
{
	GENERATED_BODY()
public:
	virtual void InitializeAttributes() {};
	virtual void PostAttributeChanged(FGameplayTag AttributeTag, float OldValue) {};

	URogueActionSystemComponent* GetOwningComponent() const;
};


DECLARE_MULTICAST_DELEGATE(FOnOutOfHealth);

UCLASS()
class URogueHealthAttributeSet : public URogueAttributeSet
{
	GENERATED_BODY()

public:
	// Fired once, when Health drops from above zero to zero
	FOnOutOfHealth OnOutOfHealth;

	UPROPERTY(EditAnywhere, category = "Attributes")
	FRogueAttribute Health;
	
	UPROPERTY(EditAnywhere, category = "Attributes")  
	FRogueAttribute HealthMax;
	
	virtual void PostAttributeChanged(FGameplayTag AttributeTag, float OldValue) override;
	URogueHealthAttributeSet();
};

UCLASS()
class URoguePawnAttributeSet : public URogueHealthAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category=Attributes)
	FRogueAttribute MoveSpeed;
	
	URoguePawnAttributeSet();
	virtual void InitializeAttributes() override;
	virtual void PostAttributeChanged(FGameplayTag AttributeTag, float OldValue) override;
	void ApplyMoveSpeed();
};

UCLASS()
class URoguePlayerAttributeSet : public URoguePawnAttributeSet
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, category = "Attributes")  
	FRogueAttribute Rage;
	
	UPROPERTY(EditAnywhere, category = "Attributes")  
	FRogueAttribute RageMax;

	// Fraction of health lost that is converted into Rage
	UPROPERTY(EditAnywhere, category = "Attributes")
	float RageGainPerDamage = 0.1f;

public:
	URoguePlayerAttributeSet();
	virtual void PostAttributeChanged(FGameplayTag AttributeTag, float OldValue) override;
};

UCLASS()
class URogueMonsterAttributeSet : public URoguePawnAttributeSet
{
	GENERATED_BODY()

public:
	URogueMonsterAttributeSet();
};