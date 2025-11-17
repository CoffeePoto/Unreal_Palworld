// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/Pokemon/PokemonEnum.h"
#include "PokemonStat.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UPokemonStat : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void SetPokemonNameandThumbnail(const FString NewPokemonName);
	void SetMaxHp(int PokemonMaxHp);
	void SetTypeImage(EPokemonType Type1, EPokemonType Type2);
	void SetLevel(int NewLevel);
	void UpdateCurrentHp(int PokemonCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class UImage> PokemonThumbnail;

	UPROPERTY()
	TObjectPtr<class UTextBlock> PokemonName;

	UPROPERTY()
	TObjectPtr<class UProgressBar> HpBar;

	UPROPERTY()
	TObjectPtr<class UImage> TypeImage1;

	UPROPERTY()
	TObjectPtr<class UImage> TypeImage2;

	UPROPERTY()
	TObjectPtr<class UTextBlock> LevelText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> MaxHpText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> CurrentHpText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pokemon Info")
	TMap<FString, UTexture2D*> PokemonIcons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pokemon Info")
	TMap<EPokemonType, UTexture2D*> TypeIcons;
};
