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

protected:

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> TEXT_Name;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> TEXT_LV;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> IMG_Type1;
	
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> IMG_Type2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pokemon Info")
    TMap<EPokemonType, UTexture2D*> TypeIcons;
};
