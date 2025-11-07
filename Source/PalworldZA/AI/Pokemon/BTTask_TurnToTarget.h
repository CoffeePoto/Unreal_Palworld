// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UBTTask_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TurnToTarget();

protected:
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;

	virtual void TickTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds
	)override;

protected:
	// 회전 폰
	UPROPERTY()
	TObjectPtr<APawn> ControllingPawn;

	// 포켓몬이 바라볼 방향.
	FVector LookVector;

	// 회전 값 .
	FRotator TargetRot;

	// 회전 속도 (Todo: 임시 값) 
	float TurnSpeed = 8.0f;
};
