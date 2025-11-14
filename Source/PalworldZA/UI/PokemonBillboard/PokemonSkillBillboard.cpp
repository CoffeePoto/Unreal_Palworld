// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PokemonBillboard/PokemonSkillBillboard.h"
#include "Character/Pokemon/PokemonBase.h"
#include "Interface/PokemonInterface/CommandReceiver.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPokemonSkillBillboard::NativeConstruct()
{
	Super::NativeConstruct();
	IMG_SkillNameBound->SetVisibility(ESlateVisibility::Hidden);
	TEXT_SkillName->SetVisibility(ESlateVisibility::Hidden);
}

void UPokemonSkillBillboard::OnSkillUI(const FString& SkillName)
{
	TEXT_SkillName->SetText(FText::FromString(SkillName));

	if (bIsTrainer)
	{
		if (Anim_PopIn)
		{
			PlayAnimation(Anim_PopIn);
		}
	}
	else
	{
		if (Anim_PopInEnemy)
		{
			PlayAnimation(Anim_PopInEnemy);
		}
	}
}

void UPokemonSkillBillboard::EndSkillUI()
{
	TEXT_SkillName->SetVisibility(ESlateVisibility::Hidden);
	if (Anim_PopOut)
	{
		PlayAnimation(Anim_PopOut);
	}
}

void UPokemonSkillBillboard::BindPokemon(APokemonBase* Pokemon)
{
	ICommandReceiver* Controller = Cast<ICommandReceiver>(Pokemon);
	if (!Controller) { return; }

	FStartPokemonSkill::FDelegate StartDel;
	StartDel.BindUObject(this, &UPokemonSkillBillboard::OnSkillUI);
	StartHandle = Controller->BindStartPokemonSkill(StartDel);

	FEndPokemonSkill::FDelegate EndDel;
	EndDel.BindUObject(this, &UPokemonSkillBillboard::EndSkillUI);
	EndHandle = Controller->BindEndPokemonSkill(EndDel);
}

void UPokemonSkillBillboard::IsTrainer(bool Check)
{
	bIsTrainer = Check;
}
