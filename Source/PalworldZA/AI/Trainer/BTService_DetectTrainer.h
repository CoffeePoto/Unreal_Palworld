// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectTrainer.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UBTService_DetectTrainer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_DetectTrainer();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
