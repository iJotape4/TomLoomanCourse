// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SDamagePopUp_Widget.generated.h"

/**
 * 
 */
UCLASS()
class TOMLOOMANCOURSE_API USDamagePopUp_Widget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	float TimeOnScreen = 2.0f;

	UPROPERTY(BlueprintReadOnly, Category="Config")
	float DamageText = 0.0f;
	
	TObjectPtr<AActor> AttachTo;
	FTimerHandle TimerHandle;
	
	void Display();
	void OnTimeElapsed();
	virtual void NativeConstruct() override;
	virtual  void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
