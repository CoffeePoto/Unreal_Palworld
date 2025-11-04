// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonAnimInstanceBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPokemonAnimInstanceBase::UPokemonAnimInstanceBase()
{
}

bool UPokemonAnimInstanceBase::SetAnimSequence()
{

	return false;
}

void UPokemonAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());

	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UPokemonAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}
