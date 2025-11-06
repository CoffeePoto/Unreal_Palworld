// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Pokemon/SkillBase.h"
#include "TestCloseRangeSkill.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API ATestCloseRangeSkill : public ASkillBase
{
	GENERATED_BODY()

public:
	ATestCloseRangeSkill();

	// 스킬 실행
	virtual void ExecuteSkill() override;


protected:
	UPROPERTY()
	TSubclassOf<AActor> FireSlash;

	// 돌진할 때 얼마나 

	// 돌진 속도.
	UPROPERTY()
	float RushSpeed;

	// 얼마나 앞까지 돌진할 지 거리.
	UPROPERTY()
	float StopDistance;
};
