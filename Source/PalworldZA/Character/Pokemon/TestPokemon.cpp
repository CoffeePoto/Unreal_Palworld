// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Pokemon/TestPokemon.h"
#include "AI/Pokemon/PokemonAIController.h"


ATestPokemon::ATestPokemon()
{
	LoadAnimSequenceData(TEXT("/Game/Data/Pokemon/DA_DarkCrowAnimSequence.DA_DarkCrowAnimSequence"));
}

void ATestPokemon::BeginPlay()
{
	Super::BeginPlay();
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
