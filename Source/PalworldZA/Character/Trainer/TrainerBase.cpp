// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Trainer/TrainerBase.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Character/Pokemon/PokemonBase.h"

// Sets default values
ATrainerBase::ATrainerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//변수 초기화
	SelectedPokemon = 0;

	//Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//캐릭터 이동 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	GetCharacterMovement()->JumpZVelocity = 700.0f;

	//Mesh 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -80.0f), FRotator(0.0f, -90.0f, 0.0f));
	//에셋 설정은 에디터에서 진행

}

void ATrainerBase::SummonPokemon()
{
	//Todo : Pokemons 배열에 담긴 포켓몬 객체 생성
	//플레이어 위치 + 조정값을 포켓몬 객체에 넘겨준다.
	FVector SummonLocation = GetActorLocation() + FVector(200.0f, 200.0f, 0.0f);
	//유효성 검사
	if (Pokemons.IsValidIndex(SelectedPokemon))
	{
		Pokemons[SelectedPokemon]->SetActive(SummonLocation);
	}
}

void ATrainerBase::CommandSkills(uint8 SkillIndex)
{
	//스킬 사용
	//유효성 검사
	if (Pokemons.IsValidIndex(SelectedPokemon))
	{
		Pokemons[SelectedPokemon]->UsingSkill(SkillIndex);
	}
}

