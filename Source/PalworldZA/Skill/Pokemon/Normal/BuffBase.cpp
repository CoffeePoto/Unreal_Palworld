// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/Normal/BuffBase.h"
#include "Interface/PokemonInterface/CommandReceiver.h"

ABuffBase::ABuffBase()
{
}

void ABuffBase::ExecuteSkill()
{
	// User Null 체크
	if (!User) { return; }

	// 동작 회복
	FTimerHandle SkillEndTimer;
	GetWorldTimerManager().SetTimer(
		SkillEndTimer,
		this,
		&ABuffBase::OnEndSkill,
		2.0f,
		false
	);
}

void ABuffBase::OnEndSkill()
{
	ICommandReceiver* PokemonController = Cast<ICommandReceiver>(User);

	if (!BuffSetting.IsEmpty())
	{
		for (FBuffSetting& Setting : BuffSetting)
		{
			PokemonController->SetBuff(
				Setting.Stat,
				Setting.Time,
				Setting.Cover
			);
		}
	}

	if (!DebuffSetting.IsEmpty())
	{
		for (FBuffSetting& Setting : DebuffSetting)
		{
			PokemonController->SetDeBuff(
				Setting.Stat,
				Setting.Time,
				Setting.Cover
			);
		}
	}

	Super::OnEndSkill();
}