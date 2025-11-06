// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TrainerBase.generated.h"

UCLASS()
class PALWORLDZA_API ATrainerBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATrainerBase();

protected:
	//selectedpokemon 변수 외부 조정을 위한 getter/setter
	FORCEINLINE uint8 GetSelectedPokemon() { return SelectedPokemon; }
	FORCEINLINE void SetSelectedPokemon(uint8 setNum) { SelectedPokemon = setNum; }

protected:
	//포켓몬 소환
	void SummonPokemon();
	//포켓몬에게 명령 하달
	void CommandSkills(/*어떤 스킬을 사용할지 결정하는 매개변수*/ uint8 SkillIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Possess)
	//트레이너가 소유하고 있는 포켓몬 list
	TArray<class APokemonBase*> Pokemons;

	//배열 안에서 포켓몬을 선택하기 위한 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Possess)
	uint8 SelectedPokemon;
};
