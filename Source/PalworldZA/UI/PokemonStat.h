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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetHpBarPortion(float DeltaTime);

public:
	void SetPokemonNameandThumbnail(const FString NewPokemonName);
	void SetMaxHp(int PokemonMaxHp);
	void SetTypeImage(EPokemonType Type1, EPokemonType Type2);
	void SetLevel(int NewLevel);
	void SetDeadInfoOff();
	void UpdateCurrentHp(int PokemonCurrentHp);
	void OnHpChanged(int NewHp);
	void ShowDeadEvent();

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
	TObjectPtr<class UTextBlock> SlashText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> CurrentHpText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> DeadInfoText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pokemon Info")
	TMap<FString, UTexture2D*> PokemonIcons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pokemon Info")
	TMap<EPokemonType, UTexture2D*> TypeIcons;

private:
	//HpBar 비율 계산을 위한 최대 체력 저장 변수
	int MaxHp = 1;
	//HpBar 선형 보간을 위한 현재 체력 저장 변수
	int CurrentHp = 1;
	int ChangedHp = 1;
};
