// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSingleton.h"

UGameSingleton::UGameSingleton()
{
	// DT_PokemonStatTable 로드.
	static ConstructorHelpers::FObjectFinder<UDataTable> StatTableRef(TEXT("/Game/Data/Pokemon/DT_PokemonStatTable.DT_PokemonStatTable"));
	if (StatTableRef.Succeeded())
	{
		const UDataTable* StatTable = StatTableRef.Object;

		// 재확인.
		ensureAlways(StatTable->GetRowMap().Num() > 0);

		// 스탯의 Table(TArray)과 Map(TMap)에 csv 파일 데이터를 채움.
		for (const auto& RowPair : StatTable->GetRowMap())
		{
			FName RowName = RowPair.Key;
			FPokemonStatData* RowData = reinterpret_cast<FPokemonStatData*>(RowPair.Value);
			PokemonStatTable.Add(*RowData);
			PokemonStatMap.Add(RowName, *RowData);
		}

		// 확인.
		NumOfPokemon = PokemonStatTable.Num();
		ensureAlways(NumOfPokemon > 0);
	}
	

	// DT_PokemonSkillTable 로드.
	static ConstructorHelpers::FObjectFinder<UDataTable> SkillTableRef(TEXT("/Game/Data/Pokemon/DT_PokemonSkillTable.DT_PokemonSkillTable"));
	if (SkillTableRef.Succeeded())
	{
		const UDataTable* SkillTable = SkillTableRef.Object;

		// 재확인.
		ensureAlways(SkillTable->GetRowMap().Num() > 0);

		// 스킬의 Table(TArray)과 Map(TMap)에 csv 파일 데이터를 채움.
		for (const auto& RowPair : SkillTable->GetRowMap())
		{
			FName RowName = RowPair.Key;
			FPokemonSkillData* RowData = reinterpret_cast<FPokemonSkillData*>(RowPair.Value);
			PokemonSkillTable.Add(*RowData);
			PokemonSkillMap.Add(RowName, *RowData);
		}
		
		// 확인.
		NumOfSkill= PokemonSkillTable.Num();
		ensureAlways(NumOfSkill > 0);

		for (const auto& Pair : PokemonStatMap)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inserted Stat Key: '%s'"), *Pair.Key.ToString());
		}

		for (const auto& Pair : PokemonSkillMap)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inserted Skill Key: '%s'"), *Pair.Key.ToString());
		}

	}
		
}

UGameSingleton& UGameSingleton::Get()
{
	UGameSingleton* Singleton = CastChecked<UGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	//UE_LOG(LogPokemonSingleton, Error, TEXT("Invalid PokemonSingleton"));
	return *NewObject<UGameSingleton>();
}

