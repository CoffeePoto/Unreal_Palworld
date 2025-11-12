// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PokemonBillboard/PokemonBillboard.h"
#include "Components/ProgressBar.h"

void UPokemonBillboard::SetHpBar(float Percent)
{
    TargetHP = FMath::Clamp(Percent, 0.f, 1.f);
}

void UPokemonBillboard::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!PB_HpBar) return;
    if (DisplayedHP == TargetHP) return;

    // 부드럽게 보간
    DisplayedHP = FMath::FInterpTo(DisplayedHP, TargetHP, InDeltaTime, HpInterpSpeed);

    // 0.01 이내면 정지
    if (FMath::Abs(TargetHP - DisplayedHP) < 0.01f)
        DisplayedHP = TargetHP;

    PB_HpBar->SetPercent(DisplayedHP);

    // 색상 (미리 캐싱된 상수)
    static const FLinearColor HpColor_High = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("B4FFABFF")));
    static const FLinearColor HpColor_Mid = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("FFF08AFF")));
    static const FLinearColor HpColor_Low = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("FF574CFF")));

    const FLinearColor& TargetColor =
        (DisplayedHP > 0.5f) ? HpColor_High :
        (DisplayedHP > 0.3f) ? HpColor_Mid :
        HpColor_Low;

    PB_HpBar->WidgetStyle.FillImage.TintColor = FSlateColor(TargetColor);
}
