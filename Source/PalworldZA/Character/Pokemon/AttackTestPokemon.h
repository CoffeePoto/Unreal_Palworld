// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Pokemon/PokemonBase.h"
#include "AttackTestPokemon.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API AAttackTestPokemon : public APokemonBase
{
	GENERATED_BODY()

public:
	AAttackTestPokemon();

protected:

	virtual void Tick(float DeltaTime) override;

	float TargetTime = 5.0f;
	float CurrentTime = 0.0f;
	int32 SkillNumber = 0;
};
