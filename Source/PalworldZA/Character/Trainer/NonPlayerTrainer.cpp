// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Trainer/NonPlayerTrainer.h"
#include "AI/Trainer/NpcAIController.h"
#include "Character/Pokemon/PokemonBase.h"

ANonPlayerTrainer::ANonPlayerTrainer()
{
	//AIController วาด็
	AIControllerClass = ANpcAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

float ANonPlayerTrainer::GetAIPatrolRadius()
{
	return 800.0f;
}

float ANonPlayerTrainer::GetAIDetectRange()
{
	return 400.0f;
}

UObject* ANonPlayerTrainer::GetPokemon()
{
	UObject* PokemonObject = Cast<UObject>(Pokemons[SelectedPokemon]);
	if (PokemonObject)
	{
		return PokemonObject;
	}
	return nullptr;
}

void ANonPlayerTrainer::AISummonPokemon()
{
	uint8 randomSelect = (uint8)FMath::FRandRange(0.0f, Pokemons.Num());
	SelectedPokemon = randomSelect;
	SummonPokemon();
}

void ANonPlayerTrainer::AICommandSkills()
{
	uint8 randomSelect = (uint8)FMath::FRandRange(0.0f, 3.0f);
	CommandSkills(randomSelect);
}
