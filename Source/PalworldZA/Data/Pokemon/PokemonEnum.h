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


UENUM(BlueprintType)
enum class EPokemonType : uint8
{
	NONE,
	NORMAL,
	FIRE,
	GRASS,
	GROUND,
	ELECTRICITY,
	WATER,
	ICE,
	DRAGON,
	DARK,
	COUNT
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

