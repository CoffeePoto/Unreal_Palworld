// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/Normal/NormalBuff.h"

ANormalBuff::ANormalBuff()
{
	Data.ActionType = EActionType::BUFF;
}

void ANormalBuff::ExecuteSkill()
{
	// User Null 체크
	if (!User) { return; }
	IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(User);

	// User 위치 가져오기.
	FVector UserPos = User->GetActorLocation();

	// 동작 회복
	FTimerHandle SkillEndTimer;
	GetWorldTimerManager().SetTimer(
		SkillEndTimer,
		this,
		&ASkillBase::OnEndSkill,
		1.0f,
		false
	);
}
