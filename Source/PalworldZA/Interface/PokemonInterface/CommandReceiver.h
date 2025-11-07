// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/Pokemon/PokemonEnum.h"
#include "CommandReceiver.generated.h"

DECLARE_DELEGATE(FOnPokemonDown);
DECLARE_MULTICAST_DELEGATE(FEndPokemonSkill);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCommandReceiver : public UInterface
{
	GENERATED_BODY()
};


class PALWORLDZA_API ICommandReceiver
{
	GENERATED_BODY()

public:

	// 스킬 사용
	virtual bool UsingSkill(uint8 SkillNumber) = 0;

	// 포켓몬 활성화 
	virtual bool SetActive(FVector Location) = 0;

	// 포켓몬 비활성화 
	virtual bool Deactive() = 0;

	// 포켓몬 쓰러졌을 때 호출할 델리게이트 등록 
	virtual void BindOnPokemonDown(const FOnPokemonDown& InDelegate) = 0;

	// 포켓몬 공격 종료시 호출할 델리게이트 등록
	virtual FDelegateHandle BindEndPokemonSkill(const FEndPokemonSkill::FDelegate& InDelegate) = 0;

	// 포켓몬 공격 종료시 호출할 델리게이트 해지
	virtual void UnBindEndPokemonSkill(FDelegateHandle Handle) = 0;

	// 포켓몬 타겟 설정
	virtual void SetTarget(AActor* NewTarget) = 0;

	// 포켓몬 트레이너 설정
	virtual void SetTrainer(APawn* NewTrainer) = 0;

	// 버프 설정 / 매개 변수 : (버프 스탯, 시간, 버프 덮어쓰기 여부)
	virtual void SetBuff(EPokemonBuffStat Stat, float Time, bool IsCover = true) = 0;

	//virtual void UsingItem() = 0;
};
