// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Pokemon/SkillBase.h"
#include "ElectricitySlash.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API AElectricitySlash : public ASkillBase
{
	GENERATED_BODY()
	
public:
	AElectricitySlash();

	// 스킬 실행
	virtual void ExecuteSkill() override;


protected:
	UPROPERTY()
	TSubclassOf<AActor> ElectricitySlash;
};
