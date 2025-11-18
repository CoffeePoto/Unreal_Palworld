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
	PokemonThumbnail->SetVisibility(ESlateVisibility::Hidden);

	PokemonName = Cast<UTextBlock>(GetWidgetFromName(TEXT("PokemonName")));
	ensureAlways(PokemonName);
	PokemonName->SetVisibility(ESlateVisibility::Hidden);

	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	ensureAlways(HpBar);
	HpBar->SetVisibility(ESlateVisibility::Hidden);

	TypeImage1 = Cast<UImage>(GetWidgetFromName(TEXT("Type1")));
	ensureAlways(TypeImage1);
	TypeImage1->SetVisibility(ESlateVisibility::Hidden);

	TypeImage2 = Cast<UImage>(GetWidgetFromName(TEXT("Type2")));
	ensureAlways(TypeImage2);
	TypeImage2->SetVisibility(ESlateVisibility::Hidden);

	LevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LevelText")));
	ensureAlways(LevelText);
	LevelText->SetVisibility(ESlateVisibility::Hidden);

	MaxHpText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxHpText")));
	ensureAlways(MaxHpText);
	MaxHpText->SetVisibility(ESlateVisibility::Hidden);

	SlashText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Slash")));
	ensureAlways(SlashText);
	SlashText->SetVisibility(ESlateVisibility::Hidden);

	CurrentHpText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentHpText")));
	ensureAlways(CurrentHpText);
	CurrentHpText->SetVisibility(ESlateVisibility::Hidden);

	DeadInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("DeadInfo")));
	ensureAlways(DeadInfoText);
	DeadInfoText->SetVisibility(ESlateVisibility::Hidden);

	UpdateCanTick();
}

void UPokemonStat::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetHpBarPortion(InDeltaTime);
}

void UPokemonStat::SetPokemonNameandThumbnail(const FString NewPokemonName)
{
	FText Name = FText::FromString(NewPokemonName);
	PokemonName->SetText(Name);
	PokemonName->SetVisibility(ESlateVisibility::Visible);

	if (UTexture2D** TryThumbnail = PokemonIcons.Find(NewPokemonName))
	{
		UTexture2D* ThumbnailImage = *TryThumbnail;
		if (ThumbnailImage && PokemonThumbnail)
		{
			PokemonThumbnail->SetBrushFromTexture(ThumbnailImage);
			PokemonThumbnail->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UPokemonStat::SetMaxHp(int PokemonMaxHp)
{
	FText FMaxHpText = FText::AsNumber(PokemonMaxHp);
	MaxHp = PokemonMaxHp;
	MaxHpText->SetText(FMaxHpText);
	MaxHpText->SetVisibility(ESlateVisibility::Visible);
	SlashText->SetVisibility(ESlateVisibility::Visible);
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
	LevelText->SetVisibility(ESlateVisibility::Visible);
}

void UPokemonStat::SetDeadInfoOff()
{
	DeadInfoText->SetVisibility(ESlateVisibility::Hidden);
}

void UPokemonStat::UpdateCurrentHp(int PokemonCurrentHp)
{
	CurrentHp = PokemonCurrentHp;
	ChangedHp = CurrentHp; //초반 변화를 막기 위함.
	HpBar->SetPercent((float)CurrentHp / MaxHp);
	HpBar->SetVisibility(ESlateVisibility::Visible);
	FText FCurrentHpText = FText::AsNumber(PokemonCurrentHp);
	CurrentHpText->SetText(FCurrentHpText);
	CurrentHpText->SetVisibility(ESlateVisibility::Visible);
}

void UPokemonStat::OnHpChanged(int NewHp)
{
	ChangedHp = NewHp;
}

void UPokemonStat::ShowDeadEvent()
{
	PokemonThumbnail->SetVisibility(ESlateVisibility::Hidden);
	PokemonName->SetVisibility(ESlateVisibility::Hidden);
	HpBar->SetVisibility(ESlateVisibility::Hidden);
	TypeImage1->SetVisibility(ESlateVisibility::Hidden);
	TypeImage2->SetVisibility(ESlateVisibility::Hidden);
	LevelText->SetVisibility(ESlateVisibility::Hidden);
	MaxHpText->SetVisibility(ESlateVisibility::Hidden);
	SlashText->SetVisibility(ESlateVisibility::Hidden);
	CurrentHpText->SetVisibility(ESlateVisibility::Hidden);
	DeadInfoText->SetVisibility(ESlateVisibility::Visible);
}

void UPokemonStat::SetHpBarPortion(float DeltaTime)
{
	CurrentHp = FMath::FInterpTo(CurrentHp, ChangedHp, DeltaTime, 3.0f);
	FText FCurrentHpText = FText::AsNumber(CurrentHp);
	CurrentHpText->SetText(FCurrentHpText);

	float Ratio = (float)CurrentHp / MaxHp;
	HpBar->SetPercent(Ratio);
}
