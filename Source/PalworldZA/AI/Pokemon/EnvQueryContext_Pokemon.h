// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_Pokemon.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UEnvQueryContext_Pokemon : public UEnvQueryContext
{
	GENERATED_BODY()

public:

protected:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

};
