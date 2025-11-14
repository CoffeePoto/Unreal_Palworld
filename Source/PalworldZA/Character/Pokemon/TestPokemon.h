// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Pokemon/PokemonBase.h"
#include "TestPokemon.generated.h"

/**
 * 적 포켓몬.
 */
UCLASS()
class PALWORLDZA_API ATestPokemon : public APokemonBase
{
	GENERATED_BODY()

public:
	ATestPokemon();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual bool UsingSkill(uint8 SkillNumber) override;

};
