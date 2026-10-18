// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAnimInstance.h"

#include "TomLoomanCourse/SharedGameplayTags.h"
#include "TomLoomanCourse/ActionSystem/URogueActionSystemComponent.h"

void URogueAnimInstance::Death(AActor* InstigatorActor)
{
	bIsAlive = false;
}

void URogueAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Actioncomp = GetOwningActor()->FindComponentByClass<URogueActionSystemComponent>(); 
}

void URogueAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Actioncomp)
		bIsSprinting = Actioncomp->ActiveGameplayTags.HasTag(SharedGameplayTags::StatusEffect_Sprinting); 	
	
}
