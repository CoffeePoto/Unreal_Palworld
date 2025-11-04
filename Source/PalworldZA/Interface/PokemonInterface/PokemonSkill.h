// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PokemonSkill.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPokemonSkill : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PALWORLDZA_API IPokemonSkill
{
	GENERATED_BODY()

public:

	// 스킬 실행
	virtual void ExecuteSkill() = 0;

	// 스킬 종료 시
	virtual void OnEndSkill() = 0;

	// 스킬 정보 반환 (Todo : 데이터 구조 잡은뒤 반환값 조정)
	virtual void GetSkillData() = 0;
};
