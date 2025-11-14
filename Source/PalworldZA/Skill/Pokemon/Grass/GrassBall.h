// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Pokemon/SkillBase.h"
#include "GrassBall.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API AGrassBall : public ASkillBase
{
	GENERATED_BODY()

public:
	AGrassBall();

	// 스킬 실행
	virtual void ExecuteSkill() override;

protected:

	// 발사체 소환
	AActor* SpawnProjectile(FVector Pos, FRotator Rot);

protected:
	UPROPERTY()
	TSubclassOf<AActor> GrassBallClass;

	UPROPERTY()
	TObjectPtr<AActor> GrassBall;
};
