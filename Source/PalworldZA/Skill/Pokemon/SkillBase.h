// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PokemonInterface/PokemonSkill.h"
#include "Data/Pokemon/PokemonSkillData.h"
#include "SkillBase.generated.h"

class APokemonBase;

UCLASS()
class PALWORLDZA_API ASkillBase : 
	public AActor,
	public IPokemonSkill
{
	GENERATED_BODY()
	
public:	
	
	ASkillBase();

	void InitializationSKill(APokemonBase* InUser);

	// 스킬 실행
	virtual void ExecuteSkill() override;

	// 스킬 종료 시
	virtual void OnEndSkill() override;

	// 스킬 정보 반환 (Todo : 데이터 구조 잡은뒤 반환값 조정)
	virtual const FPokemonSkillData& GetSkillData() override;

protected:
	
	UPROPERTY()
	TObjectPtr<APokemonBase> User;

	UPROPERTY()
	FPokemonSkillData Data;
};
