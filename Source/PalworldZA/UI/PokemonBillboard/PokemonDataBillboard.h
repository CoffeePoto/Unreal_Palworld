// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/Pokemon/PokemonEnum.h"
#include "PokemonDataBillboard.generated.h"


UCLASS()
class PALWORLDZA_API UPokemonDataBillboard : public UUserWidget
{
	GENERATED_BODY()

public:

    virtual void NativeConstruct() override;

    // 이름 업데이트
    void UpdateName(const FString& NewName);

    // 레벨 업데이트
    void UpdateLevel(int NewLevel);

    // 타입 업데이트 
    void UpdateType(EPokemonType NewType1, EPokemonType NewType2);

    // 야생 업데이트
    void UpdateWilde(bool IsWild);

    // 포켓몬 델리게이트 연결
    void SetPokemonSkillEvent(class APokemonBase* Pokemon);

    // 이름 활성화
    void ActivateName();

    // 이름 비활성화 
    void DeactivateNameForSkillEndEvent(const FString& Name);

protected:

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> TEXT_Name;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> TEXT_LV;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> IMG_Type1;
	
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> IMG_Type2;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> IMG_Wild;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pokemon Info")
    TMap<EPokemonType, UTexture2D*> TypeIcons;

private:
    // 시작 델리게이트 핸들
    FDelegateHandle StartHandle;

    // 종료 델리게이트 핸들
    FDelegateHandle EndHandle;
};
