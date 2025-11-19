// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Pokemon/SkillBase.h"
#include "GrassStorm.generated.h"

/**
 * 
 */

class AProjectileBase;

UCLASS()
class PALWORLDZA_API AGrassStorm : public ASkillBase
{
	GENERATED_BODY()

public:
	AGrassStorm();

	// 스킬 실행
	virtual void ExecuteSkill() override;

protected:

	// 발사체 소환
	AProjectileBase* SpawnProjectile(FVector Pos, FRotator Rot);

protected:
	UPROPERTY()
	TSubclassOf<AProjectileBase> GrassStormlClass;

	UPROPERTY()
	TObjectPtr<AProjectileBase> GrassStorm;
};
