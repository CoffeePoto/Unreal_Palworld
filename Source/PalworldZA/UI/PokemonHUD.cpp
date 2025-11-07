// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PokemonHUD.h"
#include "Components/HorizontalBox.h"
#include "UI/PokemonSlot.h"
#include "UI/ThumbnailPath.h"

void UPokemonHUD::NativeConstruct()
{
	Super::NativeConstruct();

	SlotBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("HorizontalBox")));
	ensureAlways(SlotBox);
	PreviousSelected = Cast<UPokemonSlot>(GetWidgetFromName(TEXT("Pokemon1")));
	ensureAlways(PreviousSelected);
	//this is for test
	PreviousSelected->SetPokemonThumbnail(FIREFOX);

	PreviousSelected->Select();
}

void UPokemonHUD::SelectPokemon(uint8 Index)
{
}
