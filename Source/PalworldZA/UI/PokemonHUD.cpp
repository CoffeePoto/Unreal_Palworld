// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PokemonHUD.h"
#include "Components/HorizontalBox.h"
#include "UI/PokemonSlot.h"
#include "UI/PokemonStat.h"
#include "UI/SkillSetting.h"


void UPokemonHUD::NativeConstruct()
{
	Super::NativeConstruct();

	SlotBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("HorizontalBox")));
	ensureAlways(SlotBox);
	UISlot = Cast<UPokemonSlot>(GetWidgetFromName(TEXT("Pokemon1")));
	ensureAlways(UISlot);
	PokemonInfo = Cast<UPokemonStat>(GetWidgetFromName(TEXT("PokemonStat")));
	ensureAlways(PokemonInfo);
	for (int i = 0; i < 4; ++i)
	{
		FString SkillIndex = FString::Printf(TEXT("Skill%d"), i + 1);
		USkillSetting* IndiviaulSkillSlot = Cast<USkillSetting>(GetWidgetFromName(FName(*SkillIndex)));
		if (IndiviaulSkillSlot)
		{
			IndiviaulSkillSlot->SetVisibility(ESlateVisibility::Hidden);
			SkillInfos.Add(IndiviaulSkillSlot);
		}
	}
}

void UPokemonHUD::SetSlotThumbnail(uint8 Index, const FString newPokemonName)
{
	FString WidgetName = FString::Printf(TEXT("Pokemon%d"), Index + 1);
	//새롭게 선택된 UI슬롯 할당
	UISlot = Cast<UPokemonSlot>(GetWidgetFromName(FName(*WidgetName)));
	UISlot->SetPokemonThumbnail(newPokemonName);
}

void UPokemonHUD::SelectUI(uint8 Index)
{
	//이전 지정되어있는 UI 슬롯 비활성화
	UISlot->UnSelect();
	FString WidgetName = FString::Printf(TEXT("Pokemon%d"), Index + 1);
	//새롭게 선택된 UI슬롯 할당
	UISlot = Cast<UPokemonSlot>(GetWidgetFromName(FName(*WidgetName)));
	if (UISlot)
	{
		UISlot->Select();
	}
}

void UPokemonHUD::SetSkillSetting(uint8 Index, FString SkillName, float CoolTime, EPokemonType SkillType)
{
	SkillInfos[Index]->SetSkillName(SkillName);
	SkillInfos[Index]->SetCoolTime(CoolTime);
	SkillInfos[Index]->SetTypeImage(SkillType);
}

void UPokemonHUD::StartSkillCoolDown(uint8 Index, float RemainingCoolTime)
{
	SkillInfos[Index]->SetCoolTime(RemainingCoolTime);
}

void UPokemonHUD::ShowSkillUI()
{
	for (int i = 0; i < 4; ++i)
	{
		SkillInfos[i]->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPokemonHUD::HideSkillUI()
{
	for (int i = 0; i < 4; ++i)
	{
		SkillInfos[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}
