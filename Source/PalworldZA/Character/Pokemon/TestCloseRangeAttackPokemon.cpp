// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCloseRangeAttackPokemon.h"
#include "TestPokemon.h"
#include "Kismet/GameplayStatics.h"

ATestCloseRangeAttackPokemon::ATestCloseRangeAttackPokemon()
{
	LoadAnimSequenceData(TEXT(
		"/Game/Data/Pokemon/DA_ElecCatAnimSequence.DA_ElecCatAnimSequence"
	));

	FSkillContainer NewSkill;
	static ConstructorHelpers::FClassFinder<AActor> NewSkillRef(TEXT(
		"/Game/BluePrint/TestPokemon/BP_TestCloseRangeSkill.BP_TestCloseRangeSkill_C"
	));
	if (NewSkillRef.Succeeded())
	{
		NewSkill.Skill = NewSkillRef.Class;
	}

	PokemonSkills.Add(NewSkill);
}


