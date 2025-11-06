// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PokemonDataGetter.generated.h"


UINTERFACE(MinimalAPI)
class UPokemonDataGetter : public UInterface
{
	GENERATED_BODY()
};


class PALWORLDZA_API IPokemonDataGetter
{
	GENERATED_BODY()

	// Todo : 내용 채우기 
public:

	// 현재 포켓몬이 바라보는 타겟
	virtual AActor* GetTarget() = 0;

	// 현재 포켓몬이 공격하는지 판단
	virtual uint8 IsOnSkill() = 0;

	virtual FVector GetShootPoint() = 0;
};
