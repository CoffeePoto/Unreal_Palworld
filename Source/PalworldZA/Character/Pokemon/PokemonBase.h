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

public: // 공개 함수

	APokemonBase();

	// 스킬 사용중인지 여부를 반환하는 함수
	uint8 GetIsOnSkill() { return bIsOnSkill; }

public:	// 인터페이스 구현부
	virtual void UsingSkill() override;

	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected: // 오버라이딩 구현부 

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// 대미지 처리 함수 오버라이드.
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

protected: // 자체 함수 구현부

protected: // Has 변수 

	// 자신을 소유한 트레이너 
	UPROPERTY()
	TObjectPtr<class APawn> Trainer;

	// BT를 제어하기 위한 BB컴포넌트
	UPROPERTY()
	TObjectPtr<class UBlackboardComponent> BBComponent;

	UPROPERTY()
	TObjectPtr<class UPokemonAnimSequenceData> AnimData;

	UPROPERTY()
	TObjectPtr<class UObject> SkillTarget;

	// Todo : PokemonBase 예상 추가 요소 
	// 기본 포켓몬 정보 Struct 
	// 노출 포켓몬 정보 Sturct (UI를 위함)
	// 현재 정보 

protected: // 파라미터 변수

	uint8 bIsOnSkill : 1;
};
