// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Pokemon/SkillBase.h"
#include "FireBall.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API AFireBall : public ASkillBase
{
	GENERATED_BODY()

public:
	AFireBall();

	// 스킬 실행
	virtual void ExecuteSkill() override;

protected:

	// 발사체 소환
	AActor* SpawnProjectile(FVector Pos, FRotator Rot);

protected:
	UPROPERTY()
	TSubclassOf<AActor> FireBallClass;	

	UPROPERTY()
	TObjectPtr<AActor> FireBall;
};
