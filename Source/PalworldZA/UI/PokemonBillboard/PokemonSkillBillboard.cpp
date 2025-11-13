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

	if (Anim_PopIn)
	{
		PlayAnimation(Anim_PopIn);
	}

	//IMG_SkillNameBound->SetVisibility(ESlateVisibility::Visible);
	TEXT_SkillName->SetVisibility(ESlateVisibility::Visible);
}

void UPokemonSkillBillboard::EndSkillUI()
{
	IMG_SkillNameBound->SetVisibility(ESlateVisibility::Hidden);
	TEXT_SkillName->SetVisibility(ESlateVisibility::Hidden);
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
