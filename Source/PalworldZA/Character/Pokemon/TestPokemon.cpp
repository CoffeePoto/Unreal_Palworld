// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Pokemon/TestPokemon.h"
#include "AI/Pokemon/PokemonAIController.h"
#include "Game/GameSingleton.h"
#include "Data/Pokemon/PokemonSkillDataAsset.h"

ATestPokemon::ATestPokemon()
{
	// DARK_CROW의 anim sequence data asset 로드.
	LoadAnimSequenceData(TEXT("/Game/Data/Pokemon/DA_DarkCrowAnimSequence.DA_DarkCrowAnimSequence"));
	
	// DARK_CROW가 사용할 AI 컨트롤러 클래스 지정.
	AIControllerClass = APokemonAIController::StaticClass();

	// 맵에 배치되거나 스폰될 때 자동으로 AI가 이 포켓몬을 조종하도록 설정.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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

	MyName = TEXT("다크 크로우");
}

void ATestPokemon::BeginPlay()
{
	Super::BeginPlay();

	// DARK_CROW Stat 정보 싱글턴에서 가져오기.
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
