#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "PokemonDamageEvent.generated.h"

USTRUCT()
struct FPokemonDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	// 반드시 포함해야 함
	static const int32 ClassID = 0xBEEFCAFE; // 임의의 유니크 ID (전역에서 충돌 안 나면 됨)

	virtual int32 GetTypeID() const override { return FPokemonDamageEvent::ClassID; }

public:
	// 공격 속성 
	UPROPERTY()
	FString Type;

	// 공격 위력
	UPROPERTY()
	float Power;

};
