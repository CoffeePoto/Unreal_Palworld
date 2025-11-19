// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Pokemon/SkillBase.h"
#include "WaterBeam.generated.h"

/**
 * 
 */

class AProjectileBase;

UCLASS()
class PALWORLDZA_API AWaterBeam : public ASkillBase
{
	GENERATED_BODY()
	
public:
	AWaterBeam();

	virtual void ExecuteSkill() override;

protected:

	// 발사체 소환
	AProjectileBase* SpawnProjectile(FVector Pos, FRotator Rot);

protected:

	UPROPERTY()
	TSubclassOf<AActor> WaterBeamClass;

	UPROPERTY()
	TObjectPtr<AProjectileBase> WaterBeam;

};
