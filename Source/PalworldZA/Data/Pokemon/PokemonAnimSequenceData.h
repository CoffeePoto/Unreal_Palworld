// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Animation/AnimSequence.h"
#include "PokemonAnimSequenceData.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UPokemonAnimSequenceData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 대기 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* IdleAnim;

	// 움직임 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* MovementAnim;

	// 스킬 사용 (시작) 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* SkillStartAnim;

	// 스킬 사용 (중간) 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* SkillLoopAnim;

	// 스킬 사용 (종료) 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* SkillEndAnim;

	// 피격 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* HitAnim;

	// 조우 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* EncountAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* DeadAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* DeadLoopAnim;
};
