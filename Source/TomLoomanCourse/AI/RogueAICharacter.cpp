// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAICharacter.h"

#include "TomLoomanCourse/ActionSystem/URogueActionSystemComponent.h"


// Sets default values
ARogueAICharacter::ARogueAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttributesComponent = CreateDefaultSubobject<URogueActionSystemComponent>("AttributesComponent");

}