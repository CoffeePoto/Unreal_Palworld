// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBuff.h"
#include "Interface/PokemonInterface/CommandReceiver.h"

AFireBuff::AFireBuff()
{
	Data.ActionType = EActionType::BUFF;
}

void AFireBuff::ExecuteSkill()
{
	// User Null 체크
	if (!User) { return; }
	Data = UGameSingleton::Get().GetPokemonSkillDataByName(TEXT("FIRE_003"));
	
	// 동작 회복
	FTimerHandle SkillEndTimer;
	GetWorldTimerManager().SetTimer(
		SkillEndTimer,
		this,
		&AFireBuff::OnEndSkill,
		2.0f,
		false
	);
}

void AFireBuff::OnEndSkill()
{
	ICommandReceiver* PokemonController = Cast<ICommandReceiver>(User);
	PokemonController->SetBuff(EPokemonBuffStat::ATK, 20.0f);

	Super::OnEndSkill();
}
