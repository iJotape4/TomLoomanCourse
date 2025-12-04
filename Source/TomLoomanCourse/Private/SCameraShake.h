// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shakes/LegacyCameraShake.h"
#include "SCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class TOMLOOMANCOURSE_API USCameraShake : public ULegacyCameraShake
{
	GENERATED_BODY()

public:
	// Sets up default legacy camera shake oscillation parameters
	USCameraShake();
};
