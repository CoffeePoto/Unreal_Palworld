// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PokemonBillboard/PokemonDataBillboard.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Character/Pokemon/PokemonBase.h"
#include "Interface/PokemonInterface/CommandReceiver.h"

void UPokemonDataBillboard::NativeConstruct()
{
    Super::NativeConstruct();
}

void UPokemonDataBillboard::UpdateName(const FString& NewName)
{
    TEXT_Name->SetText(FText::FromString(NewName));
}

void UPokemonDataBillboard::UpdateLevel(int NewLevel)
{
    FString NewLevelText = FString::Printf(TEXT("Lv.%d"), NewLevel);
    TEXT_LV->SetText(FText::FromString(NewLevelText));
}

void UPokemonDataBillboard::UpdateType(EPokemonType NewType1, EPokemonType NewType2)
{
    if (NewType1 == EPokemonType::NONE || NewType1 == EPokemonType::COUNT)
    {
        IMG_Type1->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        IMG_Type1->SetVisibility(ESlateVisibility::Visible);
        IMG_Type1->SetBrushFromTexture(TypeIcons[NewType1]);
    }

    if (NewType2 == EPokemonType::NONE || NewType2 == EPokemonType::COUNT)
    {
        IMG_Type2->SetVisibility(ESlateVisibility::Hidden);
    }
    else 
    {
        IMG_Type2->SetVisibility(ESlateVisibility::Visible);
        IMG_Type2->SetBrushFromTexture(TypeIcons[NewType2]);
    }
}

void UPokemonDataBillboard::SetPokemonSkillEvent(APokemonBase* Pokemon)
{
    ICommandReceiver* Controller = Cast<ICommandReceiver>(Pokemon);
    if (!Controller) { return; }

    FStartPokemonSkill::FDelegate StartDel;
    StartDel.BindUObject(this, &UPokemonDataBillboard::DeactivateNameForSkillEndEvent);
    StartHandle = Controller->BindStartPokemonSkill(StartDel);

    FEndPokemonSkill::FDelegate EndDel;
    EndDel.BindUObject(this, &UPokemonDataBillboard::ActivateName);
    EndHandle = Controller->BindEndPokemonSkill(EndDel);
}

void UPokemonDataBillboard::ActivateName()
{
    TEXT_Name->SetVisibility(ESlateVisibility::Visible);
}

void UPokemonDataBillboard::DeactivateNameForSkillEndEvent(const FString& Name)
{
    TEXT_Name->SetVisibility(ESlateVisibility::Hidden);
}
