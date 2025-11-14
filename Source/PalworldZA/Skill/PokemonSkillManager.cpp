// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/PokemonSkillManager.h"
#include "Data/Pokemon/PokemonSkillDataAsset.h"

TSubclassOf<ASkillBase> UPokemonSkillManager::GetSkillBP(const FName InSkillName)
{
	if (UPokemonSkillDataAsset* const* FoundDA = SkillDataAssetMap.Find(InSkillName))
	{
		return (*FoundDA)->Skill;
	}
	return nullptr;
}

