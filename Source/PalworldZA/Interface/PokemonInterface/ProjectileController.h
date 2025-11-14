// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/Pokemon/PokemonDamageEvent.h"
#include "Interface/PokemonInterface/PokemonSkill.h"
#include "ProjectileController.generated.h"

DECLARE_DELEGATE(FDestroyMember)

UINTERFACE(MinimalAPI)
class UProjectileController : public UInterface
{
	GENERATED_BODY()
};


class PALWORLDZA_API IProjectileController
{
	GENERATED_BODY()

public:

	// 총알 파괴 
	virtual void DestroyProjectile() = 0;

	// 총알이 데미지 이벤트로 넘겨줄 정보 저장
	virtual void SetDamageEvent(FPokemonDamageEvent Event) = 0;

	// 일정 시간후 총알 파괴
	virtual void SetDestroyTimer(float Sec) = 0;

	// 파괴 이벤트시 호출할 이벤트 등록
	virtual void BindDestroyEvent(const FDestroyMember& InDelegate) = 0;
};
