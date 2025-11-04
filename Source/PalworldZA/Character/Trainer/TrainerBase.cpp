// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Trainer/TrainerBase.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Character/Pokemon/PokemonBase.h"

// Sets default values
ATrainerBase::ATrainerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//캐릭터 이동 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->JumpZVelocity = 700.0f;

	//Mesh 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -80.0f), FRotator(0.0f, -90.0f, 0.0f));
	//에셋 설정은 에디터에서 진행
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

void ATrainerBase::SummonPokemon()
{
	//Todo : Pokemons 배열에 담긴 포켓몬 객체 생성

	//플레이어 위치 기준 오른쪽 위에 소환
}

void ATrainerBase::CommandSkills()
{
	//스킬 사용
}

