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

	virtual AActor* GetTarget() = 0;

};
