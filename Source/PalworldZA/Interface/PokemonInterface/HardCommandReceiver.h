// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HardCommandReceiver.generated.h"

UINTERFACE(MinimalAPI)
class UHardCommandReceiver : public UInterface
{
	GENERATED_BODY()
};

class PALWORLDZA_API IHardCommandReceiver
{
	GENERATED_BODY()

public:
	// 스킬 실행
	virtual void ExecuteSkill() = 0;

	// 스킬 예약
	virtual void ReservationSkill(int SkillNum) = 0;
 };
