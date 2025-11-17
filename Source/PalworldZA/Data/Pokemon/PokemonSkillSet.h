// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PokemonSkillSet.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UPokemonSkillSet : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PokemonSkillSetData")
	FName PokemonCodeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PokemonSkillSetData")
	FString PokemonName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PokemonSkillSetData")
	TArray<TObjectPtr<class UPokemonSkillDataAsset>> Skill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PokemonSkillSetData")
	TObjectPtr<class UPokemonAnimSequenceData> Anim;
};
