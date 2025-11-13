// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PokemonBillboard.generated.h"

class UPokemonBuffBillboard;
class UPokemonDataBillboard;
class UPokemonSkillBillboard;

UCLASS()
class PALWORLDZA_API UPokemonBillboard : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE UPokemonBuffBillboard* GetBuffBB() { return WBP_Buff; }

	FORCEINLINE UPokemonDataBillboard* GetDataBB() { return WBP_Data; }

	// HP바 설정
	void SetHpBar(float Percent);

	// 플레이어 소유 포켓몬 UI 세팅
	void SetPlayerPokemonView();

	// NPC 소유, 야생 포켓몬 UI 세팅
	void SetNonPlayerPokemonView();

	// 포켓몬 세팅
	void SetPokemon(class APokemonBase* Pokemon);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> PB_HpBar;

	UPROPERTY()
	TObjectPtr<UPokemonBuffBillboard> WBP_Buff;

	UPROPERTY()
	TObjectPtr<UPokemonDataBillboard> WBP_Data;

	UPROPERTY()
	TObjectPtr<UPokemonSkillBillboard> WBP_Skill;

private:
	float TargetHP = 1.f;
	float DisplayedHP = 1.f;
	float HpInterpSpeed = 2.5f; // 보간 속도 (값 클수록 빠름)
};
