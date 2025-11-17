// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PokemonSpawner.generated.h"

UCLASS()
class PALWORLDZA_API APokemonSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	APokemonSpawner();

protected:
	virtual void BeginPlay() override;

	// 포켓몬 스폰 세팅 및 실행
	virtual void SetSpawnPokemon();

	// 포켓몬 스폰
	virtual void SpawnPokemon(int PokemonIndex, FVector Location);

	// 스폰한 포켓몬 기절 또는 포획시
	virtual void ExitSpawnPokemon();

public:	

protected:

	// 최대 포켓몬 스폰 수 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxPokemonSpawnCount = 0;

	// 스폰 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnRadius = 10.0f;

	// 스폰 주기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTime = 10.0f;

	// 스폰 포켓몬 종류
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class APokemonBase>> SpawnPokemonTypes;

	// 현재 포켓몬 수
	int32 CurrentPokemonSpawnCount = 0;
};
