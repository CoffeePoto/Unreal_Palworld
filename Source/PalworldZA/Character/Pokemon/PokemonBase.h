// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/PokemonInterface/CommandReceiver.h"
#include "Interface/PokemonInterface/PokemonDataGetter.h"
#include "PokemonBase.generated.h"

UCLASS()
class PALWORLDZA_API APokemonBase : 
	public ACharacter,
	public ICommandReceiver,
	public IPokemonDataGetter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APokemonBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 대미지 처리 함수 오버라이드.
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

public:	
	virtual void UsingSkill() override;

	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// 자신을 소유한 트레이너 
	UPROPERTY()
	TObjectPtr<class APawn> Trainer;

	// Todo : PokemonBase 예상 추가 요소 
	// 기본 포켓몬 정보 Struct 
	// 노출 포켓몬 정보 Sturct (UI를 위함)
	// 현재 정보 
};
