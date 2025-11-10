// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PokemonHUD.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UPokemonHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SelectUI(uint8 Index);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UHorizontalBox> SlotBox;
	UPROPERTY()
	//선택 해제 전 visible한 선택 영역을 hidden하기 위한 변수
	TObjectPtr<class UPokemonSlot> UISlot;
};
