// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TrainerTurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UBTTask_TrainerTurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_TrainerTurnToTarget();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

protected:
	TObjectPtr<APawn> ControllingPawn;

	// 포켓몬이 바라볼 방향.
	FVector LookVector;

	// 회전 값 .
	FRotator TargetRot;

	// 회전 속도 (Todo: 임시 값) 
	float TurnSpeed;
};
