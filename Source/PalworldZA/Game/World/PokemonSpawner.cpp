// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/World/PokemonSpawner.h"
#include "NavigationSystem.h"
#include "Character/Pokemon/PokemonBase.h"
#include "Character/Pokemon/AttackTestPokemon.h"
#include "Data/Pokemon/PokemonSkillSet.h"
#include "Skill/Pokemon/SkillBase.h"
#include "Data/Pokemon/PokemonSkillDataAsset.h"

APokemonSpawner::APokemonSpawner()
{
	static ConstructorHelpers::FClassFinder<APokemonBase> PokemonRef(TEXT("/Game/BluePrint/TestPokemon/BP_TestPokemon.BP_TestPokemon_C"));

	if (PokemonRef.Succeeded())
	{
		SpawnPokemonTypes.Add(PokemonRef.Class);
	}
}

void APokemonSpawner::BeginPlay()
{
	Super::BeginPlay();	

	FTimerHandle SpawnTimer;

	// 타이머
	GetWorldTimerManager().SetTimer(
		SpawnTimer,
		this,
		&APokemonSpawner::SetSpawnPokemon,
		SpawnTime,
		true                     
	);
}

void APokemonSpawner::SetSpawnPokemon()
{
	if (MaxPokemonSpawnCount < CurrentPokemonSpawnCount) { return; }

	FNavLocation Result;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	if (NavSys->GetRandomPointInNavigableRadius(GetActorLocation(), SpawnRadius, Result))
	{
		int32 RandomNumber = FMath::RandRange(0, SpawnPokemonSets.Num() - 1);
		SpawnPokemon(RandomNumber, Result.Location);
	}
}

void APokemonSpawner::SpawnPokemon(int PokemonIndex, FVector Location)
{
	//UClass* SpawnPokemon = SpawnPokemonTypes[PokemonIndex];
	UClass* SpawnPokemon = SpawnPokemonSets[PokemonIndex]->Pokemon;
	FActorSpawnParameters SpawnParams;

	// 파라미터 값 세팅
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 스킬 스폰 
	APokemonBase* SpawnPk = GetWorld()->SpawnActor<APokemonBase>(
		SpawnPokemon,
		Location,
		GetActorRotation(),
		SpawnParams
	);
	if (!SpawnPk) { return; }

	ICommandReceiver* Controller = Cast<ICommandReceiver>(SpawnPk);
	if (!Controller) { return; }

	FOnPokemonDown Delegate;
	Delegate.BindUObject(this, &APokemonSpawner::ExitSpawnPokemon);

	
	TArray<UPokemonSkillDataAsset*> Skills;

	for (UPokemonSkillDataAsset* SkillClass : SpawnPokemonSets[PokemonIndex]->Skill)
	{
		Skills.Add(SkillClass);
	}

	Controller->SetPokemonData(SpawnPokemonSets[PokemonIndex]->PokemonCodeName, SpawnPokemonSets[PokemonIndex]->PokemonName);
	Controller->SetPokemonSkills(Skills);
	Controller->SetPokemonAnimData(SpawnPokemonSets[PokemonIndex]->Anim);
	Controller->BindOnPokemonDown(Delegate);

	++CurrentPokemonSpawnCount;
}

void APokemonSpawner::ExitSpawnPokemon()
{
	CurrentPokemonSpawnCount = FMath::Max(CurrentPokemonSpawnCount - 1, 0);
}