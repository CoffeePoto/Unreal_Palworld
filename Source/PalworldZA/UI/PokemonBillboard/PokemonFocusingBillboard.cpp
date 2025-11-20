// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonFocusingBillboard.h"
#include "Components/Image.h"

void UPokemonFocusingBillboard::OnImage()
{
	IMG_EyeIcon->SetVisibility(ESlateVisibility::Visible);
}

void UPokemonFocusingBillboard::OffImage()
{
	IMG_EyeIcon->SetVisibility(ESlateVisibility::Hidden);
}
