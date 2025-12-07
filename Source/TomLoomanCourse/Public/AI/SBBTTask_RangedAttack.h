// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class TOMLOOMANCOURSE_API USBBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category= "Config")
	TSubclassOf<AActor> ProjectileClass;
};
