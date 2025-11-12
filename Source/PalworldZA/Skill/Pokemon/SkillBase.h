// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/Pokemon/PokemonBase.h"
#include "Interface/PokemonInterface/PokemonSkill.h"
#include "Components/ShapeComponent.h"
#include "Data/Pokemon/PokemonDamageEvent.h"
#include "Data/Pokemon/PokemonSkillData.h"
#include "Game/GameSingleton.h"
#include "SkillBase.generated.h"

class APokemonBase;

UCLASS()
class PALWORLDZA_API ASkillBase : 
	public AActor,
	public IPokemonSkill
{
	GENERATED_BODY()
	
public:	
	
	ASkillBase();

	void InitializeSkill(APokemonBase* InUser);

	// 스킬 실행
	virtual void ExecuteSkill() override;

	// 스킬 종료 시
	UFUNCTION()
	virtual void OnEndSkill() override;

	// 스킬 정보 반환
	virtual const FPokemonSkillData& GetSkillData() override;

	// 유저 세팅
	virtual void SetUser(APokemonBase* NewUser) override;

protected:

	// 데미지 계산 처리 함수
	float DamageCalculator();

	// 데이터 로드 함수
	bool SkillDataLoad();

	// 콜리전 검색 함수
	UShapeComponent* SearchCollision(FString Name);

	// 데미지 이벤트 생성 함수
	FPokemonDamageEvent MakeDamageEvent();

protected:

	UPROPERTY()
	TObjectPtr<APokemonBase> User;

	UPROPERTY()
	FPokemonSkillData Data;
	
};
