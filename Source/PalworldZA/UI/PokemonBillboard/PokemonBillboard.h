// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PokemonBillboard.generated.h"

class UPokemonBuffBillboard;
class UPokemonDataBillboard;

UCLASS()
class PALWORLDZA_API UPokemonBillboard : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE UPokemonBuffBillboard* GetBuffBB() { return WBP_Buff; }

	FORCEINLINE UPokemonDataBillboard* GetDataBB() { return WBP_Data; }

	void SetHpBar(float Percent);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> PB_HpBar;

	UPROPERTY()
	TObjectPtr<UPokemonBuffBillboard> WBP_Buff;

	UPROPERTY()
	TObjectPtr<UPokemonDataBillboard> WBP_Data;

private:
	float TargetHP = 1.f;
	float DisplayedHP = 1.f;
	float HpInterpSpeed = 2.5f; // 보간 속도 (값 클수록 빠름)
};
