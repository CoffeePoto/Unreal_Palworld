// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SummonPokemon.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UBTTask_SummonPokemon : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SummonPokemon();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
