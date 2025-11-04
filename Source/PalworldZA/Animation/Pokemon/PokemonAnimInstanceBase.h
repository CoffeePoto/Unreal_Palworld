// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PokemonAnimInstanceBase.generated.h"

/**
 * 
 */

USTRUCT()
struct FAnimSequencePath 
{
	GENERATED_BODY()

public:
	FString IdlePath;
	FString MovementPath;
	FString SkillStart;
	FString SkillLoop;
	FString SkillEnd;
	FString Hit;
};


UCLASS()
class PALWORLDZA_API UPokemonAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPokemonAnimInstanceBase();
	
	bool SetAnimSequence();

protected:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY()
	TObjectPtr<class UCharacterMovementComponent> Movement;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	uint8 bEndFlag : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	uint8 bIsOnHit : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	uint8 bIsOnSkill : 1;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimSequence* IdleAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimSequence* MovementAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimSequence* SkillStartAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimSequence* SkillLoopAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimSequence* SKillEndAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimSequence* HitAnim;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	//UAnimSequence* DeadAnim;
};
