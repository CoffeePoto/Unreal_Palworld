// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Pokemon/SkillBase.h"
#include "LightningBase.generated.h"

/**
 * Lightning 계열 스킬의 베이스 코드.
 */

// 이펙트 or 투사체의 베이스 클래스 전방선언.
class AProjectileBase;

UCLASS()
class PALWORLDZA_API ALightningBase : public ASkillBase
{
	GENERATED_BODY()

public:
	ALightningBase();

	// 스킬 실행
	virtual void ExecuteSkill() override;

protected:

	// 발사체 소환
	AProjectileBase* SpawnProjectile(FVector Pos, FRotator Rot);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<AProjectileBase> LightningClass;

	UPROPERTY()
	TObjectPtr<AProjectileBase> Lightning;
};
