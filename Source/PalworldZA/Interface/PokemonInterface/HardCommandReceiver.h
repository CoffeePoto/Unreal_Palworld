// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HardCommandReceiver.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHardCommandReceiver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PALWORLDZA_API IHardCommandReceiver
{
	GENERATED_BODY()

public:
	// 스킬 실행
	virtual void ExecuteSkill() = 0;
};
