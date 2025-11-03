// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonBase.h"
#include "AI/Pokemon/PokemonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
APokemonBase::APokemonBase()
{
	//PrimaryActorTick.bCanEverTick = true;

	// 포켓몬 AI 컨트롤러 생성
	AIControllerClass = APokemonAIController::StaticClass();
	
	// 모두 AIController에 빙의되도록 설정.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void APokemonBase::PostInitializeComponents()
{
}

// Called when the game starts or when spawned
void APokemonBase::BeginPlay()
{
	Super::BeginPlay();

	BBComponent = Cast<APokemonAIController>(GetController())->GetBlackboardComponent();
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

