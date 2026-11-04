// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAICharacter.h"

#include "TomLoomanCourse/SharedGameplayTags.h"
#include "TomLoomanCourse/ActionSystem/URogueActionSystemComponent.h"


// Sets default values
ARogueAICharacter::ARogueAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>("ActionSystemComponent");
	ActionSystemComponent->SetDefaultAttributeSet(URogueMonsterAttributeSet::StaticClass());
}

float ARogueAICharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, -ActualDamage);

	return ActualDamage;
}
