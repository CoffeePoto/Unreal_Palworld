// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PokemonAIController.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API APokemonAIController : public AAIController
{
	GENERATED_BODY()

public:
	APokemonAIController();

	// AI 시작 함수
	void RunAI();

	// AI 정지 함수
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	// 블랙 보드 애셋 
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	// 행동 트리 애셋
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
