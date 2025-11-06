// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/SkillBase.h"
#include "Character/Pokemon/PokemonBase.h"

// Sets default values
ASkillBase::ASkillBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

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
