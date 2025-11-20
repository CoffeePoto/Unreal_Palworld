// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/Pokemon/PokemonEnum.h"
#include "PokemonHUD.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UPokemonHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetSlotThumbnail(uint8 Index, const FString newPokemonName);
	void SelectUI(uint8 Index);

	void SetSkillSetting(uint8 Index, FString SkillName, float CoolTime, EPokemonType SkillType);
	void StartSkillCoolDown(uint8 Index, float RemainingCoolTime);
	void ShowSkillUI();
	void HideSkillUI();

	FORCEINLINE class UPokemonSlot* GetUISlot() { return UISlot; }
	FORCEINLINE class UPokemonStat* GetStatUI() { return PokemonInfo; }

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UHorizontalBox> SlotBox;
	UPROPERTY()
	//선택 해제 전 visible한 선택 영역을 hidden하기 위한 변수
	TObjectPtr<class UPokemonSlot> UISlot;

	UPROPERTY()
	TObjectPtr<class UPokemonStat> PokemonInfo;

	UPROPERTY()
	TArray<TObjectPtr<class USkillSetting>> SkillInfos;
};
