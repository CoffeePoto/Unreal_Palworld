// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonAnimInstanceBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/PokemonInterface/PokemonDataGetter.h"

UPokemonAnimInstanceBase::UPokemonAnimInstanceBase()
{
}

bool UPokemonAnimInstanceBase::SetAnimSequence(const UPokemonAnimSequenceData* AnimData)
{
	if (!AnimData)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetAnimSequence: AnimData is null."));
		return false;
	}

	IdleAnim		=	AnimData->IdleAnim;
	MovementAnim	=	AnimData->MovementAnim;
	SkillStartAnim	=	AnimData->SkillStartAnim;
	SkillLoopAnim	=	AnimData->SkillLoopAnim;
	SKillEndAnim	=	AnimData->SkillEndAnim;
	HitAnim			=	AnimData->HitAnim;
	EncountAnim		=	AnimData->EncountAnim;
	DeadAnim		=	AnimData->DeadAnim;
	DeadLoopAnim	=	AnimData->DeadLoopAnim;

	return true;
}

void UPokemonAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());

	OwnerGetter.SetObject(Owner);
	OwnerGetter.SetInterface(Cast<IPokemonDataGetter>(Owner));

	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UPokemonAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		MovementSpeed = Movement->Velocity.Size2D();

		
		if (OwnerGetter)
		{
			bIsOnSkill = OwnerGetter->IsOnSkill();
			bIsOnDead = OwnerGetter->GetIsPokemonDown();
		}
	}
}
