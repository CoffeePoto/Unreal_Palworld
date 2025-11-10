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

		// 편의를 위해 배열로 변환해서 처리.
		TArray<uint8*> StatValueArray;
		StatTable->GetRowMap().GenerateValueArray(StatValueArray);

		// 형변환해서 배열에 추가.
		Algo::Transform(StatValueArray, PokemonStatTable, [](uint8* Value)
			{
				return *reinterpret_cast<FPokemonStatData*>(Value);
			}
		);

		// 이름 기반으로 빠른 접근이 가능하도록 TMap 구성.
		for (const FPokemonStatData& Data : PokemonStatTable)
		{
			PokemonStatMap.Add(Data.RowName, Data);
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

		// 편의를 위해 배열로 변환해서 처리.
		TArray<uint8*> SkillValueArray;
		SkillTable->GetRowMap().GenerateValueArray(SkillValueArray);

		// 형변환해서 배열에 추가.
		Algo::Transform(SkillValueArray, PokemonSkillTable, [](uint8* Value)
			{
				return *reinterpret_cast<FPokemonSkillData*>(Value);
			}
		);

		// 이름 기반으로 빠른 접근이 가능하도록 TMap 구성. but FString을 FName으로 형변환.
		for (const FPokemonSkillData& Data : PokemonSkillTable)
		{
			PokemonSkillMap.Add(Data.RowName, Data);
		}
		
		// 확인.
		NumOfSkill= PokemonSkillTable.Num();
		ensureAlways(NumOfSkill > 0);
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

