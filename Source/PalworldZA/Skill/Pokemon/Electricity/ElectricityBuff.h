// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Pokemon/SkillBase.h"
#include "ElectricityBuff.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API AElectricityBuff : public ASkillBase
{
	GENERATED_BODY()
	
public:
	AElectricityBuff();

	// 스킬 실행
	virtual void ExecuteSkill() override;


protected:
	UPROPERTY()
	TSubclassOf<AActor> ElectricityBuff;
};
