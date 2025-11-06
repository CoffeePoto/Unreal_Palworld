// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/SkillBase.h"
#include "Character/Pokemon/PokemonBase.h"


ASkillBase::ASkillBase()
{
 	

}

void ASkillBase::InitializeSkill(APokemonBase* InUser)
{
	User = InUser;
}

void ASkillBase::ExecuteSkill()
{
}

void ASkillBase::OnEndSkill()
{
	User->EndSkill();
}

const FPokemonSkillData& ASkillBase::GetSkillData()
{
	return Data;
}
