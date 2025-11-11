// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Pokemon/AttackTestPokemon.h"
#include "AttackTestPokemon.h"
#include "TestPokemon.h"
#include "Character/Trainer/PlayerTrainer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Pokemon/PokemonBBKeys.h"
#include "Game/GameSingleton.h"
#include "Kismet/GameplayStatics.h"

AAttackTestPokemon::AAttackTestPokemon()
{
	LoadAnimSequenceData(TEXT("/Game/Data/Pokemon/DA_FireFoxAnimSequence.DA_FireFoxAnimSequence"));

	PokemonSkills.Empty();

	FSkillContainer NewSkill;

	// @Todo: 새로운 테스트 스킬 적용시 여기다 작성
	static ConstructorHelpers::FClassFinder<AActor> NewSkillRef(TEXT("/Game/BluePrint/PokemonSkill/Fire/BP_FireSlash.BP_FireSlash_C"));
	if (NewSkillRef.Succeeded())
	{
		NewSkill.Skill = NewSkillRef.Class;
	}

	PokemonSkills.Add(NewSkill);

	static ConstructorHelpers::FClassFinder<AActor> NewSkillRef2(TEXT("/Game/BluePrint/PokemonSkill/Fire/BP_FireBall.BP_FireBall_C"));
	if (NewSkillRef2.Succeeded())
	{
		NewSkill.Skill = NewSkillRef2.Class;
	}

	PokemonSkills.Add(NewSkill);

	static ConstructorHelpers::FClassFinder<AActor> NewSkillRef3(TEXT("/Game/BluePrint/PokemonSkill/Fire/BP_FireBuff.BP_FireBuff_C"));
	if (NewSkillRef3.Succeeded())
	{
		NewSkill.Skill = NewSkillRef3.Class;
	}

	PokemonSkills.Add(NewSkill);

	//static ConstructorHelpers::FClassFinder<AActor> NewSkillRef4(TEXT("/Game/BluePrint/TestPokemon/BP_TestFourthSkill.BP_TestFourthSkill_C"));
	//if (NewSkillRef4.Succeeded())
	//{
	//	NewSkill.Skill = NewSkillRef4.Class;
	//}
	//
	//PokemonSkills.Add(NewSkill);
}

void AAttackTestPokemon::BeginPlay()
{
	Super::BeginPlay();
	DefaultStatData = UGameSingleton::Get().GetPokemonStatData(1);
	
	CurrentHP = DefaultStatData.Hp;
}

void AAttackTestPokemon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	CurrentTime += DeltaTime;
	if (CurrentTime >= TargetTime)
	{
		if (CurrentSkillTarget)
		{
			CurrentTime = 0.0f;
			UsingSkill(SkillNumber);
			SkillNumber = (SkillNumber + 1) % 3;
		}
	}
	*/

	CurrentSkillTarget = UGameplayStatics::GetActorOfClass(GetWorld(), ATestPokemon::StaticClass());
	NewSkillTarget = CurrentSkillTarget;
	BBComponent->SetValueAsObject(BBKEY_TARGET_OBJECT, NewSkillTarget);

	/*
	if (!Trainer)
	{
		Trainer = Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerTrainer::StaticClass()));
		BBComponent->SetValueAsObject(BBKEY_OWNER, Trainer);
	}
	*/
}

