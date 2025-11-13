// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Data/Pokemon/PokemonAnimSequenceData.h"
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
	
	// 애니메이션 시퀀스를 설정하는 함수
	bool SetAnimSequence(const UPokemonAnimSequenceData* AnimData);

protected:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY()
	TScriptInterface<class IPokemonDataGetter> OwnerGetter;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	uint8 bIsOnDead : 1;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimSequence* EncountAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimSequence* DeadAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimSequence* DeadLoopAnim;
};
