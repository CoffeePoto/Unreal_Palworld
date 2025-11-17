// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PokemonStat.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Engine/Texture2D.h"

void UPokemonStat::NativeConstruct()
{
	PokemonThumbnail = Cast<UImage>(GetWidgetFromName(TEXT("PokemonThumbnail")));
	ensureAlways(PokemonThumbnail);

	PokemonName = Cast<UTextBlock>(GetWidgetFromName(TEXT("PokemonName")));
	ensureAlways(PokemonName);

	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	ensureAlways(HpBar);

	TypeImage1 = Cast<UImage>(GetWidgetFromName(TEXT("Type1")));
	ensureAlways(TypeImage1);

	TypeImage2 = Cast<UImage>(GetWidgetFromName(TEXT("Type2")));
	ensureAlways(TypeImage2);

	LevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LevelText")));
	ensureAlways(LevelText);

	MaxHpText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxHpText")));
	ensureAlways(MaxHpText);

	CurrentHpText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentHpText")));
	ensureAlways(CurrentHpText);
}

void UPokemonStat::SetPokemonNameandThumbnail(const FString NewPokemonName)
{
	FText Name = FText::FromString(NewPokemonName);
	PokemonName->SetText(Name);

	if (UTexture2D** TryThumbnail = PokemonIcons.Find(NewPokemonName))
	{
		UTexture2D* ThumbnailImage = *TryThumbnail;
		if (ThumbnailImage && PokemonThumbnail)
		{
			PokemonThumbnail->SetBrushFromTexture(ThumbnailImage);
		}
	}
}

void UPokemonStat::SetMaxHp(int PokemonMaxHp)
{
	FText MaxHp = FText::AsNumber(PokemonMaxHp);
	MaxHpText->SetText(MaxHp);
}

void UPokemonStat::SetTypeImage(EPokemonType Type1, EPokemonType Type2)
{
	if (Type1 == EPokemonType::NONE || Type1 == EPokemonType::COUNT)
	{
		TypeImage1->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TypeImage1->SetVisibility(ESlateVisibility::Visible);
		TypeImage1->SetBrushFromTexture(TypeIcons[Type1]);
	}

	if (Type2 == EPokemonType::NONE || Type2 == EPokemonType::COUNT)
	{
		TypeImage2->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TypeImage2->SetVisibility(ESlateVisibility::Visible);
		TypeImage2->SetBrushFromTexture(TypeIcons[Type2]);
	}
}

void UPokemonStat::SetLevel(int NewLevel)
{
	FString NewLevelText = FString::Printf(TEXT("Lv.%d"), NewLevel);
	LevelText->SetText(FText::FromString(NewLevelText));
}

void UPokemonStat::UpdateCurrentHp(int PokemonCurrentHp)
{
	FText CurrentHp = FText::AsNumber(PokemonCurrentHp);
	CurrentHpText->SetText(CurrentHp);
}
