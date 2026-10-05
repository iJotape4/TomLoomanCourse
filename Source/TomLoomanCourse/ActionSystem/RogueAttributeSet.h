// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RogueAttributeSet.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FRogueAttribute
{
	GENERATED_BODY()

	FRogueAttribute() {}

	FRogueAttribute(float InBase) : Base(InBase) {}

	/* The base value, such as 'Strength' that was granted by the RPG class you picked, and modified permanently during gameplay (eg. +1 Str for a Level Up or a +1 permanent boost by consuming an item). This would be 'saved to disk'. */
	UPROPERTY(EditDefaultsOnly)
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
};
