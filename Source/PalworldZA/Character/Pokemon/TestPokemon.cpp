// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Pokemon/TestPokemon.h"
#include "AI/Pokemon/PokemonAIController.h"
#include "Game/GameSingleton.h"


ATestPokemon::ATestPokemon()
{
	LoadAnimSequenceData(TEXT("/Game/Data/Pokemon/DA_DarkCrowAnimSequence.DA_DarkCrowAnimSequence"));
	
	AIControllerClass = APokemonAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MyName = TEXT("다크 크로우");
}

void ATestPokemon::BeginPlay()
{
	Super::BeginPlay();
	DefaultStatData = UGameSingleton::Get().GetPokemonStatDataByName(TEXT("DARK_CROW"));
	DefaultStatData.Type1 = EPokemonType::DARK;
	DefaultStatData.Type2 = EPokemonType::NORMAL;
	CurrentHP = DefaultStatData.Hp;
}

void ATestPokemon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ATestPokemon::UsingSkill(uint8 SkillNumber)
{
	bool Value = Super::UsingSkill(SkillNumber);

	// 나중에 포켓몬의 스킬 로직 구현
	UE_LOG(LogTemp, Warning, TEXT("TestPokemon 스킬 사용"));
	return Value;
}
