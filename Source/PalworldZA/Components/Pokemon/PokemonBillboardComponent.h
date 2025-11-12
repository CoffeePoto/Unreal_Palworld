// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Character/Pokemon/PokemonBase.h"
#include "PokemonBillboardComponent.generated.h"

/**
 * 
 */
			
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PALWORLDZA_API UPokemonBillboardComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UPokemonBillboardComponent();

protected:

	virtual void BeginPlay() override;

	virtual void TickComponent(
		float DeltaTime, 
		ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction
	) override;

protected:
	void LocationBillboard();

	void UpdateName();

	void UpdateLv();

	void UpdateType();

	void UpdateBuff();

	void UpdateHp();

protected:
	
	UPROPERTY()
	TObjectPtr<APokemonBase> Pokemon;

	UPROPERTY()
	TObjectPtr<class UPokemonBillboard> Billboard;
};
