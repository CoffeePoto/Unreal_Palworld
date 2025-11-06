// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PokemonSlot.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UPokemonSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY()
	TObjectPtr<class UImage> PokemonThumbnail;

	UPROPERTY()
	TObjectPtr<class UBorder> SelectedBorder;

	UPROPERTY()
	TObjectPtr<class UImage> KeyCode;
};
