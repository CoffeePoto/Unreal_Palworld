// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NpcAIController.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API ANpcAIController : public AAIController
{
	GENERATED_BODY()

public:
	ANpcAIController();

	void RunAI();

	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
