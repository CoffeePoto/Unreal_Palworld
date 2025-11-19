// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Pokemon/PokemonBase.h"
#include "BrokenProp.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API ABrokenProp : public APokemonBase
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

	// 대미지 처리 함수 오버라이드.
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

protected:

	virtual void PokemonDown() override;

protected:
	UPROPERTY()
	TObjectPtr<class UGeometryCollectionComponent> GeometryComponent;
	
};
