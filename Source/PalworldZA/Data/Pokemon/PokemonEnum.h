// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PokemonEnum.generated.h"


UENUM(BlueprintType)
enum class EActionType : uint8
{
	MELEE,
	RANGE,
	BUFF
};

// @Todo: uint32로 바꿔야되는데 전체적으로 바꿔야함.
UENUM(BlueprintType)
enum class EPokemonType : uint8
{
	NONE,
	NORMAL,
	FIRE,
	WATER,
	GRASS,
	ELECTRIC,
	GROUND
};

UENUM(BlueprintType)
enum class EPokemonBuffStat : uint8
{
	ATK,
	DEF,
	SPA,
	SPD,
	SPEED,
	COUNT
};

