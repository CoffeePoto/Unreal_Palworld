// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/Pokemon/PokemonStatData.h"
#include "PokemonDataGetter.generated.h"


UINTERFACE(MinimalAPI)
class UPokemonDataGetter : public UInterface
{
	GENERATED_BODY()
};


class PALWORLDZA_API IPokemonDataGetter
{
	GENERATED_BODY()


public:

	// 현재 포켓몬이 바라보는 타겟
	virtual AActor* GetTarget() = 0;

	// 현재 포켓몬이 공격하는지 판단
	virtual uint8 IsOnSkill() = 0;

	// 포켓몬 발사체 발사 위치 (Todo : 안쓰는 중)
	virtual FVector GetShootPoint() = 0;

	// 포켓몬 기본 스텟 정보 
	virtual const FPokemonStatData& GetPokemonDefaultStat() = 0;

	// 포켓몬 현재 스텟 정보
	virtual const FPokemonStatData GetPokemonCurrentStat() = 0;

	// 포켓몬 현재 체력 반환
	virtual const float GetPokemonHp() = 0;

	// 현재 포켓몬의 트레이너 반환
	virtual const APawn* GetTrainer() = 0;

	// 현재 포켓몬의 버프 상태 반환
	virtual const TArray<int8>& GetBuffState() const = 0;

	// 포켓몬 이름 반환
	virtual const FString GetPokemonName() = 0;

	// 포켓몬 기절 여부
	virtual const bool GetIsPokemonDown() = 0;
};
