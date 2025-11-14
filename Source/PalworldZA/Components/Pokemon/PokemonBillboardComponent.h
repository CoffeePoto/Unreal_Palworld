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

	// 빌보드 카메라 주시 방향으로 회전
	void LocationBillboard();

	// 이름 UI 업데이트
	void UpdateName();

	// LV UI 업데이트
	void UpdateLv();

	// 타입 UI 업데이트
	void UpdateType();

	// 버프 UI 업데이트
	void UpdateBuff();

	// HP바 업데이트
	void UpdateHp();

	// 야생 업데이트
	void UpdateWild();

	// 소유자 확인후 노출 정보 업데이트
	void CheackOwner();

protected:
	
	UPROPERTY()
	TObjectPtr<APokemonBase> Pokemon;

	UPROPERTY()
	TObjectPtr<class UPokemonBillboard> Billboard;

	bool IsNeedViewer = true;
};
