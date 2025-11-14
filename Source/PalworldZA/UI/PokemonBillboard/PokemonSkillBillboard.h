// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PokemonSkillBillboard.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API UPokemonSkillBillboard : public UUserWidget
{
	GENERATED_BODY()


public:

	virtual void NativeConstruct() override;

	// UI 출력
	void OnSkillUI(const FString& SkillName);

	// UI 비활성
	void EndSkillUI();

	// 델리게이트 등록
	void BindPokemon(class APokemonBase* Pokemon);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> IMG_SkillNameBound;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TEXT_SkillName;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Anim_PopIn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Anim_PopInEnemy;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Anim_PopOut;

private:
	// 시작 델리게이트 핸들
	FDelegateHandle StartHandle;
	
	// 종료 델리게이트 핸들
	FDelegateHandle EndHandle;
};
