// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PokemonSlot.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Engine/Texture2D.h"

void UPokemonSlot::NativeConstruct()
{
	Super::NativeConstruct();

	PokemonThumbnail = Cast<UImage>(GetWidgetFromName(TEXT("PokemonImage")));
	ensureAlways(PokemonThumbnail);
	PokemonThumbnail->SetVisibility(ESlateVisibility::Hidden);

	SelectedBorder = Cast<UBorder>(GetWidgetFromName(TEXT("SelectedSlot")));
	ensureAlways(SelectedBorder);
	SelectedBorder->SetVisibility(ESlateVisibility::Hidden);

	KeyCode = Cast<UImage>(GetWidgetFromName(TEXT("KeyCodeImage")));
	ensureAlways(KeyCode);
	KeyCode->SetVisibility(ESlateVisibility::Hidden);
}

void UPokemonSlot::SetPokemonThumbnail(const FString NewPokemonThumbnail)
{
	if (UTexture2D** TryThumbnail = PokemonIcons.Find(NewPokemonThumbnail))
	{
		UTexture2D* ThumbnailImage = *TryThumbnail;
		if (ThumbnailImage && PokemonThumbnail)
		{
			PokemonThumbnail->SetBrushFromTexture(ThumbnailImage);
			PokemonThumbnail->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UPokemonSlot::Select()
{
	SelectedBorder->SetVisibility(ESlateVisibility::Visible);
	KeyCode->SetVisibility(ESlateVisibility::Visible);
}

void UPokemonSlot::UnSelect()
{
	SelectedBorder->SetVisibility(ESlateVisibility::Hidden);
	KeyCode->SetVisibility(ESlateVisibility::Hidden);
}
