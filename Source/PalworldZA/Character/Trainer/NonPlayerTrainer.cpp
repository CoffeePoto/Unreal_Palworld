// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Trainer/NonPlayerTrainer.h"
#include "AI/Trainer/NpcAIController.h"

ANonPlayerTrainer::ANonPlayerTrainer()
{
	//AIController วาด็
	AIControllerClass = ANpcAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

float ANonPlayerTrainer::GetAIPatrolRadius()
{
	return 0.0f;
}

float ANonPlayerTrainer::GetAIDetectRange()
{
	return 0.0f;
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
