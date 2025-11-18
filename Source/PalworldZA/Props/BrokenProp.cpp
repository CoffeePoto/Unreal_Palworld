// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/BrokenProp.h"
#include "AIController.h"
#include "BrainComponent.h"

void ABrokenProp::BeginPlay()
{
	Super::BeginPlay();

	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		UBrainComponent* Brain = AI->GetBrainComponent();

		if (Brain)
		{
			Brain->StopLogic(TEXT("This is Props"));
		}
	}
}

void ABrokenProp::PokemonDown()
{
	Destroy();
}
