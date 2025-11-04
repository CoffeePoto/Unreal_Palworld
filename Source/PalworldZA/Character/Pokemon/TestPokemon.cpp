// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Pokemon/TestPokemon.h"
#include "AI/Pokemon/PokemonAIController.h"

ATestPokemon::ATestPokemon()
{
	// 포켓몬 AIController 설정.
	AIControllerClass = APokemonAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ATestPokemon::BeginPlay()
{
	Super::BeginPlay();
}

void ATestPokemon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestPokemon::UsingSkill()
{
	Super::UsingSkill();

	// 나중에 포켓몬의 스킬 로직 구현
	UE_LOG(LogTemp, Warning, TEXT("TestPokemon 스킬 사용"));
}
