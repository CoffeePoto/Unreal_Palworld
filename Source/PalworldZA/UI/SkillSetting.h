// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/Pokemon/PokemonEnum.h"
#include "SkillSetting.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API USkillSetting : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void SetSkillName(FString InSkillName);
	void SetTypeImage(EPokemonType InType);
	void SetCoolTime(float RestCoolTime);

private:
	void UpdateCoolTime();

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> SkillCoolTime;

	UPROPERTY()
	TObjectPtr<class UTextBlock> SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Icons)
	TMap<EPokemonType, UTexture2D*> TypeIcons;

private:
	FTimerHandle SkillCoolDownTimer;
	float MaxCoolTime = 0.0f;
	float RemainingCoolTime = 0.0f;
};
