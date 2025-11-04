// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TrainerController.h"

ATrainerController::ATrainerController()
{
}

void ATrainerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
