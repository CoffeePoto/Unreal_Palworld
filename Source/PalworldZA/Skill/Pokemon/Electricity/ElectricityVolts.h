// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Pokemon/SkillBase.h"
#include "ElectricityVolts.generated.h"

/**
 * 
 */

class AProjectileBase;

UCLASS()
class PALWORLDZA_API AElectricityVolts : public ASkillBase
{
	GENERATED_BODY()

public:
	AElectricityVolts();

	// 스킬 실행
	virtual void ExecuteSkill() override;

protected:

	// 발사체 소환
	AProjectileBase* SpawnProjectile(FVector Pos, FRotator Rot);

protected:

	UPROPERTY()
	TSubclassOf<AProjectileBase> ElectricityVoltsClass;

	UPROPERTY()
	TObjectPtr<AProjectileBase> ElectricityVolts;
	
};
