// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PokemonEnum.h"
#include "PokemonSkillData.generated.h"


USTRUCT()
struct FPokemonSkillData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	EPokemonType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	float DMG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	float Cooltime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	EActionType ActionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	float Range;
};