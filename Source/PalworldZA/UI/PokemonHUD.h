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
	
protected:
	virtual void NativeConstruct() override;

	void SelectPokemon(uint8 Index);

protected:
	UPROPERTY()
	TObjectPtr<class UHorizontalBox> SlotBox;
	UPROPERTY()
	TObjectPtr<class UPokemonSlot> CurrentSelected;
	UPROPERTY()
	TObjectPtr<class UPokemonSlot> PreviousSelected;
};
