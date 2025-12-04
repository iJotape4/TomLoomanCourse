// Fill out your copyright notice in the Description page of Project Settings.

#include "SCameraShake.h"

USCameraShake::USCameraShake()
{
	// Overall timing
	OscillationDuration     = 0.25f;
	OscillationBlendInTime  = 0.05f;
	OscillationBlendOutTime = 0.10f;

	// Rotational shake (degrees)
	RotOscillation.Pitch.Amplitude       = 5.0f;
	RotOscillation.Pitch.Frequency       = 25.0f;
	RotOscillation.Pitch.InitialOffset   = EInitialOscillatorOffset::EOO_OffsetRandom;

	RotOscillation.Yaw.Amplitude         = 5.0f;
	RotOscillation.Yaw.Frequency         = 25.0f;
	RotOscillation.Yaw.InitialOffset     = EInitialOscillatorOffset::EOO_OffsetRandom;

	RotOscillation.Roll.Amplitude        = 2.5f;
	RotOscillation.Roll.Frequency        = 35.0f;
	RotOscillation.Roll.InitialOffset    = EInitialOscillatorOffset::EOO_OffsetRandom;

	// Positional shake (units)
	LocOscillation.X.Amplitude           = 1.5f;
	LocOscillation.X.Frequency           = 35.0f;
	LocOscillation.X.InitialOffset       = EInitialOscillatorOffset::EOO_OffsetRandom;

	LocOscillation.Y.Amplitude           = 1.5f;
	LocOscillation.Y.Frequency           = 35.0f;
	LocOscillation.Y.InitialOffset       = EInitialOscillatorOffset::EOO_OffsetRandom;

	LocOscillation.Z.Amplitude           = 1.5f;
	LocOscillation.Z.Frequency           = 45.0f;
	LocOscillation.Z.InitialOffset       = EInitialOscillatorOffset::EOO_OffsetRandom;
	
	FOVOscillation.Amplitude            = 1.0f;
	FOVOscillation.Frequency            = 10.0f;
	FOVOscillation.InitialOffset        = EInitialOscillatorOffset::EOO_OffsetRandom;
}