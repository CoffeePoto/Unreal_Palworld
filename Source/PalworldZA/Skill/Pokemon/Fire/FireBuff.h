// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Pokemon/SkillBase.h"
#include "FireBuff.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API AFireBuff : public ASkillBase
{
	GENERATED_BODY()

public:
	AFireBuff();

	// 스킬 실행
	virtual void ExecuteSkill() override;

	virtual void OnEndSkill() override;


protected:
	UPROPERTY()
	TSubclassOf<AActor> FireBuff;
};
