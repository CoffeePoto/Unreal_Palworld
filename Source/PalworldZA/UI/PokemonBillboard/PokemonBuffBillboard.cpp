// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PokemonBillboard/PokemonBuffBillboard.h"
#include "PokemonBuffBillboard.h"
#include "Data/Pokemon/PokemonEnum.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

void UPokemonBuffBillboard::NativeConstruct()
{

}

void UPokemonBuffBillboard::UpdateStatBuff(const TArray<int8>& BuffState)
{
	int Sum = 0;

	Sum += ChangeColor(IMG_ATK, BuffState[(uint8)EPokemonBuffStat::ATK]);
	Sum += ChangeColor(IMG_DEF, BuffState[(uint8)EPokemonBuffStat::DEF]);
	Sum += ChangeColor(IMG_SPA, BuffState[(uint8)EPokemonBuffStat::SPA]);
	Sum += ChangeColor(IMG_SPD, BuffState[(uint8)EPokemonBuffStat::SPD]);
	Sum += ChangeColor(IMG_SPEED, BuffState[(uint8)EPokemonBuffStat::SPEED]);

	if (Sum > 0)
	{
		HB_BuffSlot->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HB_BuffSlot->SetVisibility(ESlateVisibility::Hidden);
	}
}

int8 UPokemonBuffBillboard::ChangeColor(UImage* Member, int8 Buff)
{
	if (Buff == 1)
	{
		FColor Color = FColor::FromHex(TEXT("FF6045FF"));
		Member->SetColorAndOpacity(FLinearColor(Color));
		return 1;
	}
	else if(Buff == -1)
	{
		FColor Color = FColor::FromHex(TEXT("2FFFEAFF"));
		Member->SetColorAndOpacity(FLinearColor(Color));
		return 1;
	}
	else
	{
		FColor Color = FColor::FromHex(TEXT("FFFFFFFF"));
		Member->SetColorAndOpacity(FLinearColor(Color));
		return 0;
	}
}


