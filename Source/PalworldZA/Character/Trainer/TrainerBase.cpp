// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Trainer/TrainerBase.h"

// Sets default values
ATrainerBase::ATrainerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrainerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrainerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATrainerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

