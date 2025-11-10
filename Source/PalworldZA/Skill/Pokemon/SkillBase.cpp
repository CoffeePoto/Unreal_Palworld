// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/SkillBase.h"
#include "Character/Pokemon/PokemonBase.h"
#include "Data/Pokemon/PokemonStatData.h"

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

void ASkillBase::SetUser(APokemonBase* NewUser)
{
	User = NewUser;
}

float ASkillBase::DamageCalculator()
{
	FPokemonStatData Stat = User->GetPokemonCurrentStat();
	float AttackTypeDMGStat;

	if (Data.ActionType == EActionType::MELEE)
	{
		AttackTypeDMGStat = Stat.ATK;
	}
	else if(Data.ActionType == EActionType::RANGE)
	{
		AttackTypeDMGStat = Stat.SPA;
	}

	float FinalDamage = AttackTypeDMGStat * (Data.DMG * 0.01f);

	if (Data.Type == Stat.Type1 || Data.Type == Stat.Type2)
	{
		FinalDamage *= 1.5f;
	}

	return FinalDamage;
}

bool ASkillBase::SkillDataLoad()
{
	return false;
}

UShapeComponent* ASkillBase::SearchCollision(FString Name)
{

	TArray<UShapeComponent*> HitCollisions;
	GetComponents<UShapeComponent>(HitCollisions);

	for (UShapeComponent* Collision : HitCollisions)
	{
		if (Collision && Collision->GetName().Contains(Name))
		{
			return Collision;
		}
	}
	return nullptr;
}

FPokemonDamageEvent ASkillBase::MakeDamageEvent()
{
	FPokemonDamageEvent DamageEvent;

	DamageEvent.Pokemon = User;
	DamageEvent.Trainer = User->GetTrainer();
	DamageEvent.Type = Data.Type;
	DamageEvent.Power = DamageCalculator();
	DamageEvent.IsSpecial = Data.ActionType == EActionType::MELEE ? false : true;

	return DamageEvent;
}
