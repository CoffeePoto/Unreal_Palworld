// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PokemonBuffBillboard.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UPokemonBuffBillboard : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void UpdateStatBuff(const TArray<int8>& BuffState);

protected:

	int8 ChangeColor(class UImage* Member, int8 Buff);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> HB_BuffSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> IMG_ATK;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> IMG_DEF;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> IMG_SPA;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> IMG_SPD;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> IMG_SPEED;
};
