// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Pokemon/AttackTestPokemon.h"
#include "AttackTestPokemon.h"
#include "TestPokemon.h"
#include "Character/Trainer/PlayerTrainer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Pokemon/PokemonBBKeys.h"
#include "Game/GameSingleton.h"
#include "Kismet/GameplayStatics.h"
#include "Data/Pokemon/PokemonSkillDataAsset.h"

AAttackTestPokemon::AAttackTestPokemon()
{
	LoadAnimSequenceData(TEXT("/Game/Data/Pokemon/DA_FireFoxAnimSequence.DA_FireFoxAnimSequence"));

	PokemonSkills.Empty();

	FSkillContainer NewSkill;

	// 스킬1: 
	static ConstructorHelpers::FObjectFinder<UPokemonSkillDataAsset> NewSkillRef(TEXT("/Game/Data/Skill/Fire/DA_FireBall.DA_FireBall"));
	if (NewSkillRef.Succeeded())
	{
		NewSkill.Skill = NewSkillRef.Object;
	}

	PokemonSkills.Add(NewSkill);

	
	static ConstructorHelpers::FObjectFinder<UPokemonSkillDataAsset> NewSkillRef2(TEXT("/Game/Data/Skill/Fire/DA_FireSlash.DA_FireSlash"));
	if (NewSkillRef2.Succeeded())
	{
		NewSkill.Skill = NewSkillRef2.Object;
	}

	PokemonSkills.Add(NewSkill);

	static ConstructorHelpers::FObjectFinder<UPokemonSkillDataAsset> NewSkillRef3(TEXT("/Game/Data/Skill/Fire/DA_FireBuff.DA_FireBuff"));
	if (NewSkillRef3.Succeeded())
	{
		NewSkill.Skill = NewSkillRef3.Object;
	}

	PokemonSkills.Add(NewSkill);

	//static ConstructorHelpers::FClassFinder<AActor> NewSkillRef4(TEXT("/Game/BluePrint/TestPokemon/BP_TestFourthSkill.BP_TestFourthSkill_C"));
	//if (NewSkillRef4.Succeeded())
	//{
	//	NewSkill.Skill = NewSkillRef4.Class;
	//}
	//
	//PokemonSkills.Add(NewSkill);

	MyName = TEXT("파이어 폭스");
}

void AAttackTestPokemon::BeginPlay()
{
	Super::BeginPlay();
	DefaultStatData = UGameSingleton::Get().GetPokemonStatData(1);
	
	CurrentHP = DefaultStatData.Hp;
}

