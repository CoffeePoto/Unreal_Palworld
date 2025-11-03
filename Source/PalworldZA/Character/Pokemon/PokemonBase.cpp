// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonBase.h"

// Sets default values
APokemonBase::APokemonBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APokemonBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APokemonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float APokemonBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

void APokemonBase::UsingSkill()
{

}

// Called to bind functionality to input
//void APokemonBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//}

