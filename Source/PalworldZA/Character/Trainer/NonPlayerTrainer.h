// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Trainer/TrainerBase.h"
#include "Interface/TrainerInterface/NPTrainerAIInterface.h"
#include "NonPlayerTrainer.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API ANonPlayerTrainer 
	: public ATrainerBase,
	public INPTrainerAIInterface
{
	GENERATED_BODY()
	
public:
	ANonPlayerTrainer();

protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual UObject* GetPokemon() override;
	virtual void AISummonPokemon() override;
	virtual void AICommandSkills() override;
};
