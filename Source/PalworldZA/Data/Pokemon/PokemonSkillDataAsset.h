// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Skill/Pokemon/SkillBase.h"
#include "PokemonSkillData.h"
#include "PokemonSkillDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UPokemonSkillDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
	FName SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
	TSubclassOf<ASkillBase> Skill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
	FPokemonSkillData Data;

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "SkillData")
	void UpdateFromCSV();
#endif
};
