// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PokemonSkill.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UBTTask_PokemonSkill : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PokemonSkill();

	// 테스크 실행
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 테스크 종료
	virtual void OnTaskFinished(
		UBehaviorTreeComponent& OwnerComp, 
		uint8* NodeMemory, 
		EBTNodeResult::Type TaskResult
	) override;

	void FinishSkillTask();


protected:

	// 스킬 종료 델리게이트 핸들러
	FDelegateHandle SkillEndEventHandle;

	// 소유자 BT 컴포넌트 
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> OwnerComponent;
};
