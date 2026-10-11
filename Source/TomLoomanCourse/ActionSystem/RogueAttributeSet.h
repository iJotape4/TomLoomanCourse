// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	FRogueAttribute(float InBase) : Base(InBase) {}

	/* The base value, such as 'Strength' that was granted by the RPG class you picked, and modified permanently during gameplay (eg. +1 Str for a Level Up or a +1 permanent boost by consuming an item). This would be 'saved to disk'. */
	UPROPERTY(EditAnywhere)
	float Base = 0.0f;

	/* Temporary modifier from buffs/debuffs, equipped items. This would not be 'saved to disk' as items would re-apply themselves on load */
	UPROPERTY(Transient)
	float Modifier = 0.0f;

	/* All game logic should get the value through here */
	float GetValue() const
	{
		// always clamp public value to zero, you could opt to make this a bool per attribute
		return FMath::Max(Base + Modifier, 0.0f);
	}
};

UCLASS()
class TOMLOOMANCOURSE_API URogueAttributeSet : public UObject
{
	GENERATED_BODY()
public:
	virtual void InitializeAttributes() {};
	virtual void PostAttributeChanged() {};
	
	URogueActionSystemComponent* GetOwningComponent() const;
};


UCLASS()
class URogueHealthAttributeSet : public URogueAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, category = "Attributes")
	FRogueAttribute Health;
	
	UPROPERTY(EditAnywhere, category = "Attributes")  
	FRogueAttribute HealthMax;
	
	virtual void PostAttributeChanged() override;
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
	virtual void PostAttributeChanged() override;
	void ApplyMoveSpeed();
};

UCLASS()
class URoguePlayerAttributeSet : public URoguePawnAttributeSet
{
	GENERATED_BODY()

public:
};

UCLASS()
class URogueMonsterAttributeSet : public URoguePawnAttributeSet
{
	GENERATED_BODY()

public:
	URogueMonsterAttributeSet();
};