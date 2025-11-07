// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Pokemon/AttackTestPokemon.h"
#include "AttackTestPokemon.h"
#include "TestPokemon.h"
#include "Character/Trainer/PlayerTrainer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Pokemon/PokemonBBKeys.h"
#include "Kismet/GameplayStatics.h"

AAttackTestPokemon::AAttackTestPokemon()
{
	LoadAnimSequenceData(TEXT("/Game/Data/Pokemon/DA_FireFoxAnimSequence.DA_FireFoxAnimSequence"));

	PokemonSkills.Empty();

	FSkillContainer NewSkill;

	// @Todo: 새로운 테스트 스킬 적용시 여기다 작성
	static ConstructorHelpers::FClassFinder<AActor> NewSkillRef(TEXT("/Game/BluePrint/TestPokemon/BP_TestBuffSkill.BP_TestBuffSkill_C"));
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
	BBComponent->SetValueAsObject(BBKEY_TARGET_OBJECT, NewSkillTarget);

	if (!Trainer)
	{
		Trainer = Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerTrainer::StaticClass()));
		BBComponent->SetValueAsObject(BBKEY_OWNER, Trainer);
	}
}

