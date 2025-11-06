// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Pokemon/AttackTestPokemon.h"
#include "AttackTestPokemon.h"
#include "TestPokemon.h"
#include "Kismet/GameplayStatics.h"

AAttackTestPokemon::AAttackTestPokemon()
{
	LoadAnimSequenceData(TEXT("/Game/Data/Pokemon/DA_FireFoxAnimSequence.DA_FireFoxAnimSequence"));

	FSkillContainer NewSkill;

	static ConstructorHelpers::FClassFinder<AActor> NewSkillRef(TEXT("/Game/BluePrint/TestPokemon/BP_TestSkill.BP_TestSkill_C"));
	if (NewSkillRef.Succeeded())
	{
		NewSkill.Skill = NewSkillRef.Class;
	}

	PokemonSkills.Add(NewSkill);
}

void AAttackTestPokemon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTime += DeltaTime;
	if (CurrentTime >= TargetTime)
	{
		if (CurrentSkillTarget)
		{
			CurrentTime = 0.0f;
			UsingSkill(0);
		}
	}

	CurrentSkillTarget = UGameplayStatics::GetActorOfClass(GetWorld(), ATestPokemon::StaticClass());
	NewSkillTarget = CurrentSkillTarget;
	
}

