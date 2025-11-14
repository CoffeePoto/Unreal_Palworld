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
	UISlot = Cast<UPokemonSlot>(GetWidgetFromName(TEXT("Pokemon1")));
	ensureAlways(UISlot);
	//this is for test
	UISlot->SetPokemonThumbnail(FIREFOX);

	UISlot->Select();
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
