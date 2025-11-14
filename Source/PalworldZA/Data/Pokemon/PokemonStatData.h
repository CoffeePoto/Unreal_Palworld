// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PokemonEnum.h"
#include "PokemonStatData.generated.h"


USTRUCT()
struct FPokemonStatData : public FTableRowBase
{
	GENERATED_BODY()

	// 포켓몬 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FName RowName;

	// 제 1 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	EPokemonType Type1;

	// 제 2 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	EPokemonType Type2;

	// 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Hp;

	// 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float ATK;

	// 방어
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float DEF;

	// 특수 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float SPA;

	// 특수 방어
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float SPD;

	// 속도 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Speed;
};