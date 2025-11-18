// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Pokemon/SkillBase.h"
#include "BuffBase.generated.h"

USTRUCT(BlueprintType)
struct FBuffSetting
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPokemonBuffStat Stat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Cover;
};

UCLASS()
class PALWORLDZA_API ABuffBase : public ASkillBase
{
	GENERATED_BODY()

public:

	ABuffBase();

	// 스킬 실행
	virtual void ExecuteSkill() override;

	virtual void OnEndSkill() override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	TArray<FBuffSetting> BuffSetting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	TArray<FBuffSetting> DebuffSetting;
};
