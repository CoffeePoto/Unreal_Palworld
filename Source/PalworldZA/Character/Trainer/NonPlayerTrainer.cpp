// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Trainer/NonPlayerTrainer.h"
#include "AI/Trainer/NpcAIController.h"
#include "Character/Pokemon/PokemonBase.h"

ANonPlayerTrainer::ANonPlayerTrainer()
{
	//AIController 할당
	AIControllerClass = ANpcAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//for (int i = 0; i < 3; ++i)
	//{
	//	FActorSpawnParameters SpawnParams;

	//	// 파라미터 값 세팅
	//	SpawnParams.Owner = this;
	//	SpawnParams.Instigator = GetInstigator();
	//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//	//uint8 randPokemonIndex = FMath::RandRange(0, 5);
	//	// 포켓몬 스폰 
	//	CurrentPokemon = GetWorld()->SpawnActor<APokemonBase>(
	//		PokemonClassArray[i],
	//		this->GetActorLocation(),
	//		GetActorRotation(),
	//		SpawnParams
	//	);

	//	if (CurrentPokemon)
	//	{
	//		CurrentPokemon->SetTrainer(this);
	//		CurrentPokemon->Deactive();

	//		Pokemons.Add(CurrentPokemon);
	//	}
	//}
}

void ANonPlayerTrainer::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < 3; ++i)
	{
		FActorSpawnParameters SpawnParams;

		// 파라미터 값 세팅
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// 포켓몬 스폰 
		CurrentPokemon = GetWorld()->SpawnActor<APokemonBase>(
			PokemonClassArray[i],
			this->GetActorLocation(),
			GetActorRotation(),
			SpawnParams
		);

		if (CurrentPokemon)
		{
			CurrentPokemon->SetTrainer(this);
			CurrentPokemon->Deactive();

			Pokemons.Add(CurrentPokemon);
		}
	}
}

float ANonPlayerTrainer::GetAIPatrolRadius()
{
	return 800.0f;
}

float ANonPlayerTrainer::GetAIDetectRange()
{
	return 800.0f;
}

float ANonPlayerTrainer::GetAITurnSpeed()
{
	return 2.0f;
}

UObject* ANonPlayerTrainer::GetPokemon()
{
	if (Pokemons.IsEmpty()) return nullptr;
	//UObject* PokemonObject = Cast<UObject>(Pokemons[SelectedPokemon]);
	UObject* PokemonObject = Cast<UObject>(CurrentPokemon);
	if (PokemonObject)
	{
		return PokemonObject;
	}
	return nullptr;
}

void ANonPlayerTrainer::SetAITarget(UObject* TargetPokemon)
{
	APokemonBase* Target = Cast<APokemonBase>(TargetPokemon);
	if (Target)
	{
		UE_LOG(LogTemp, Log, TEXT("적포켓몬 지정 완료."))
		CurrentPokemon->SetTarget(Target);
	}
}

void ANonPlayerTrainer::AISummonPokemon()
{
	uint8 randomSelect = FMath::RandRange(0, Pokemons.Num());
	SelectedPokemon = randomSelect;
	SummonPokemon();
}

void ANonPlayerTrainer::AICommandSkills()
{
	uint8 randomSelect = FMath::RandRange(0, 3);
	CommandSkills(randomSelect);
}
