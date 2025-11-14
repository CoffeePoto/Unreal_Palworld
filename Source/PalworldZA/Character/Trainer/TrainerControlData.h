// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TrainerControlData.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UTrainerControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UBTService_DetectTrainer();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
