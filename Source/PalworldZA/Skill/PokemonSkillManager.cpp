// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/PokemonSkillManager.h"
#include "Data/Pokemon/PokemonSkillDataAsset.h"

UPokemonSkillManager* UPokemonSkillManager::Get()
{
	static UPokemonSkillManager* SkillManager = nullptr;
	if (!SkillManager)
	{
		SkillManager = NewObject<UPokemonSkillManager>(GetTransientPackage());
		SkillManager->AddToRoot();
	}
	
	return SkillManager;
}

TSubclassOf<ASkillBase> UPokemonSkillManager::GetSkillBP(const FName InSkillName)
{
	if (UPokemonSkillDataAsset* const* FoundDA = SkillDataAssetMap.Find(InSkillName))
	{
		return (*FoundDA)->Skill;
	}
	return nullptr;
}