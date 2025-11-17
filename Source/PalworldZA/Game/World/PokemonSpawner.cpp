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
	if (MaxPokemonSpawnCount <= CurrentPokemonSpawnCount) { return; }

	FNavLocation Result;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	if (NavSys->GetRandomPointInNavigableRadius(GetActorLocation(), SpawnRadius, Result))
	{
		int32 RandomNumber = FMath::RandRange(0, SpawnPokemonTypes.Num() - 1);
		SpawnPokemon(RandomNumber, Result.Location);
	}
}

void APokemonSpawner::SpawnPokemon(int PokemonIndex, FVector Location)
{
	UClass* SpawnPokemon = SpawnPokemonTypes[PokemonIndex];
	FActorSpawnParameters SpawnParams;

	// 파라미터 값 세팅
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 포켓몬 스폰 
	APokemonBase* SpawnPk = GetWorld()->SpawnActor<APokemonBase>(
		SpawnPokemon,
		Location,
		GetActorRotation(),
		SpawnParams
	);
	if (!SpawnPk) { return; }

	// 인터페이스 호출
	ICommandReceiver* Controller = Cast<ICommandReceiver>(SpawnPk);
	if (!Controller) { return; }

	// 델리게이트 등록
	FOnPokemonDown Delegate;
	Delegate.BindUObject(this, &APokemonSpawner::ExitSpawnPokemon);
	Controller->BindOnPokemonDown(Delegate);

	// 현재 포켓몬 증가
	++CurrentPokemonSpawnCount;
}

void APokemonSpawner::ExitSpawnPokemon()
{
	CurrentPokemonSpawnCount = FMath::Max(CurrentPokemonSpawnCount - 1, 0);
}