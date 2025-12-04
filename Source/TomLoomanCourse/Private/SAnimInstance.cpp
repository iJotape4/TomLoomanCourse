// Fill out your copyright notice in the Description page of Project Settings.


#include "SAnimInstance.h"

void USAnimInstance::Death(AActor* InstigatorActor)
{
	bIsAlive = false;
}
