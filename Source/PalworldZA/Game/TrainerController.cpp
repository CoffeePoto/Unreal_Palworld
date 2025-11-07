// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TrainerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/PokemonHUD.h"

ATrainerController::ATrainerController()
{
	static ConstructorHelpers::FClassFinder<UPokemonHUD> HUDWidgetRef(TEXT("/Game/UI/BluePrint/WBP_HUD.WBP_HUD_C"));
	if (HUDWidgetRef.Succeeded())
	{
		HUDWidgetClass = HUDWidgetRef.Class;
	}
}

void ATrainerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	HUDWidget = CreateWidget<UPokemonHUD>(this, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}
}
