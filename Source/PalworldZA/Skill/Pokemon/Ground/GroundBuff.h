// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Pokemon/SkillBase.h"
#include "GroundBuff.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API AGroundBuff : public ASkillBase
{
	GENERATED_BODY()

public:
	AGroundBuff();

	// 스킬 실행
	virtual void ExecuteSkill() override;


protected:
	UPROPERTY()
	TSubclassOf<AActor> GroundBuff;
};
