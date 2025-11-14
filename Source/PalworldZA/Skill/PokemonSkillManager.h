// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Skill/Pokemon/SkillBase.h"
#include "PokemonSkillManager.generated.h"

/**
 *
 */

UCLASS()
class PALWORLDZA_API UPokemonSkillManager : public UObject
{
	GENERATED_BODY()
	
public:

	// FName -> BP 반환.
	UFUNCTION(BlueprintCallable)
	TSubclassOf<ASkillBase> GetSkillBP(const FName InSkillName);

protected:
	UPROPERTY(EditAnywhere)
	TMap<FName, class UPokemonSkillDataAsset*> SkillDataAssetMap;
	
};
