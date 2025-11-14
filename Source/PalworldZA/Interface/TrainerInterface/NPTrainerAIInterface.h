// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NPTrainerAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNPTrainerAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PALWORLDZA_API INPTrainerAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual UObject* GetPokemon() = 0;
	virtual void AISummonPokemon() = 0;
	virtual void AICommandSkills() = 0;
};
