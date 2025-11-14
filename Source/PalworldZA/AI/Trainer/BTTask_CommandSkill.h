// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CommandSkill.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UBTTask_CommandSkill : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_CommandSkill();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
