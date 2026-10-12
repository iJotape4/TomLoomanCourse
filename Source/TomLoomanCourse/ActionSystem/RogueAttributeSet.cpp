// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAttributeSet.h"

#include "URogueActionSystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TomLoomanCourse/SharedGameplayTags.h"

URogueHealthAttributeSet::URogueHealthAttributeSet()
{
	Health = FRogueAttribute(100);
	HealthMax = FRogueAttribute(Health.GetValue());
}

void URogueHealthAttributeSet::PostAttributeChanged(FGameplayTag AttributeTag, float OldValue)
{
	Health.Base = FMath::Clamp(Health.Base, 0.0f, HealthMax.GetValue());

	// Only fire on the hit that crosses zero
	if (AttributeTag == SharedGameplayTags::Attribute_Health && OldValue > 0.0f && Health.GetValue() <= 0.0f)
	{
		OnOutOfHealth.Broadcast();
	}
}

URoguePawnAttributeSet::URoguePawnAttributeSet()
{
	MoveSpeed = FRogueAttribute(550);
}

void URoguePawnAttributeSet::InitializeAttributes()
{
	Super::InitializeAttributes();
	ApplyMoveSpeed();
}

void URoguePawnAttributeSet::PostAttributeChanged(FGameplayTag AttributeTag, float OldValue)
{
	Super::PostAttributeChanged(AttributeTag, OldValue);
	ApplyMoveSpeed();
}

void URoguePawnAttributeSet::ApplyMoveSpeed()
{
	ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningComponent()->GetOwner());
 	OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed.GetValue();
}

URoguePlayerAttributeSet::URoguePlayerAttributeSet()
{
	RageMax = FRogueAttribute(20);
	Rage = FRogueAttribute(0);
}

void URoguePlayerAttributeSet::PostAttributeChanged(FGameplayTag AttributeTag, float OldValue)
{
	Super::PostAttributeChanged(AttributeTag, OldValue);

	if (AttributeTag == SharedGameplayTags::Attribute_Health)
	{
		const float DamageTaken = OldValue - Health.GetValue();
		if (DamageTaken > 0.0f)
		{
			GetOwningComponent()->ApplyAttributeChange(SharedGameplayTags::Attribute_Rage, DamageTaken * RageGainPerDamage, Modifier);
		}
	}

	Rage.Base = FMath::Clamp(Rage.Base, 0.0f, RageMax.GetValue());
}

URogueMonsterAttributeSet::URogueMonsterAttributeSet()
{
	MoveSpeed = FRogueAttribute(550);
}

URogueActionSystemComponent* URogueAttributeSet::GetOwningComponent() const
{
	return Cast<URogueActionSystemComponent>(GetOuter());
}