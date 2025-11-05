// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PAGameMode.h"
#include "TrainerController.h"

APAGameMode::APAGameMode()
{
	// Search a blueprint character asset.
	static ConstructorHelpers::FClassFinder<APawn> TrainerClassRef(
		TEXT("/Game/BluePrint/BP_PlayerTrainer2.BP_PlayerTrainer2_C")
	);
	
	if (TrainerClassRef.Succeeded())
	{
		// Set Default Pawn Class.
		DefaultPawnClass = TrainerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APawn> PlayerControllerClassRef(
		TEXT("/Script/PalworldZA.TrainerController")
	);
	if (PlayerControllerClassRef.Succeeded())
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
