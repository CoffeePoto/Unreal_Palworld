// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PokemonSkillManager.generated.h"

/**
 *
 */

class ASkillBase;
class UPokemonSkillDataAsset;

UCLASS(Blueprintable, BlueprintType)
class PALWORLDZA_API UPokemonSkillManager : public UObject
{
	GENERATED_BODY()
	
public:
	static UPokemonSkillManager* Get();

	// FName(스킬 이름) -> BP 반환.
	UFUNCTION(BlueprintCallable)
	TSubclassOf<ASkillBase> GetSkillBP(const FName InSkillName);

protected:
	// 블루프린트에서 넣어줄 Skill DA 리스트.
	//UPROPERTY(EditAnywhere, Category = "SKill")
	//TArray<UPokemonSkillDataAsset*> SkillDataAssetArray;

	// SkillName -> DataAsset
	UPROPERTY(EditAnywhere)
	TMap<FName, class UPokemonSkillDataAsset*> SkillDataAssetMap;
	
};