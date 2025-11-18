// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Pokemon/PokemonBase.h"
#include "BrokenProp.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API ABrokenProp : public APokemonBase
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

protected:

	virtual void PokemonDown() override;
	
};
