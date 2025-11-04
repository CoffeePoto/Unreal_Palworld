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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//포켓몬 소환
	void SummonPokemon();
	//포켓몬에게 명령 하달
	void CommandSkills(/*어떤 스킬을 사용할지 결정하는 매개변수*/);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Possess)
	//트레이너가 소유하고 있는 포켓몬 list
	TArray<class APokemonBase*> Pokemons;
};
