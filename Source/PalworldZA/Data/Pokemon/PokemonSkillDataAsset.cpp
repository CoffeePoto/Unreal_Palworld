// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Pokemon/PokemonSkillDataAsset.h"
#include "Game/GameSingleton.h"

#if WITH_EDITOR

void UPokemonSkillDataAsset::UpdateFromCSV()
{
    static FString Path = TEXT("/Game/Data/Pokemon/DT_PokemonSkillTable.DT_PokemonSkillTable");
    UDataTable* Table = LoadObject<UDataTable>(nullptr, *Path);

    if (!Table) { return; }

    FPokemonSkillData* Row = Table->FindRow<FPokemonSkillData>(SkillName, TEXT(""));
    if (Row)
    {
        Data = *Row;
    }

    MarkPackageDirty();
}

#endif