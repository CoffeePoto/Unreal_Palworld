// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Pokemon/PokemonEnum.h"

#define TYPE_COUNT (uint8)EPokemonType::COUNT

class PALWORLDZA_API FPokemonTypeTable
{
public:
    static const float Table[TYPE_COUNT][TYPE_COUNT];

    static float GetMultiplier(EPokemonType Attack, EPokemonType Defense)
    {
        return Table[(uint8)Attack][(uint8)Defense];
    }
};
