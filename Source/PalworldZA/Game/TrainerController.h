// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TrainerController.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API ATrainerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATrainerController();

	virtual void BeginPlay() override;
};
