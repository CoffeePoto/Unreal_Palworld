// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PokemonFocusingBillboard.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UPokemonFocusingBillboard : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void OnImage();

	void OffImage();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> IMG_EyeIcon;
};
