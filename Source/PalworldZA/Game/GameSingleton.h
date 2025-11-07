// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Pokemon/PokemonStatData.h"
#include "Data/Pokemon/PokemonSkillData.h"
#include "GameSingleton.generated.h"


/**
 * 
 */
UCLASS()
class PALWORLDZA_API UGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UGameSingleton();
	static UGameSingleton& Get();

	// 특정 포켓몬 스탯, 스킬 Getter.
	FORCEINLINE FPokemonStatData GetPokemonStatData(int32 InNum) const
	{
		return PokemonStatTable.IsValidIndex(InNum - 1) ? PokemonStatTable[InNum - 1] : FPokemonStatData();
	}
	FORCEINLINE FPokemonSkillData GetPokemonSkillData(int32 InNum) const
	{
		return PokemonSkillTable.IsValidIndex(InNum - 1) ? PokemonSkillTable[InNum - 1] : FPokemonSkillData();
	}

	// 수치 확인을 위한 값.
	UPROPERTY()
	int32 NumOfPokemon;
	UPROPERTY()
	int32 NumOfSkill;

private:

	// 모든 포켓몬 스탯, 스킬 데이터 배열로 가져오기.
	TArray<FPokemonStatData> PokemonStatTable;
	TArray<FPokemonSkillData> PokemonSkillTable;
	
};
