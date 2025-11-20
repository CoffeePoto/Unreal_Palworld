// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillSetting.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USkillSetting::NativeConstruct()
{
	SkillCoolTime = Cast<UProgressBar>(GetWidgetFromName(TEXT("SkillCoolTime")));
	ensureAlways(SkillCoolTime);
	SkillCoolTime->SetPercent(0.0f);

	SkillName = Cast<UTextBlock>(GetWidgetFromName(TEXT("SkillName")));
	ensureAlways(SkillName);
}

void USkillSetting::SetSkillName(FString InSkillName)
{
	FText FSkillName = FText::FromString(InSkillName);
	SkillName->SetText(FSkillName);
}

void USkillSetting::SetTypeImage(EPokemonType InType)
{
	if (InType == EPokemonType::NONE || InType == EPokemonType::COUNT)
	{
		SkillCoolTime->WidgetStyle.BackgroundImage = FSlateBrush();
	}
	else
	{
		SkillCoolTime->WidgetStyle.BackgroundImage.SetResourceObject(TypeIcons[InType]);
	}
}

void USkillSetting::SetCoolTime(float RestCoolTime)
{
	MaxCoolTime = RestCoolTime;

    UpdateCoolTime();
}

void USkillSetting::UpdateCoolTime()
{
    // 스킬 쿨타임 시작값 설정 (예: 스킬 눌렀을 때 호출)
    RemainingCoolTime = MaxCoolTime;

    // 이미 타이머가 돌고 있으면 중복 실행 방지
    if (GetWorld()->GetTimerManager().IsTimerActive(SkillCoolDownTimer))
    {
        return;
    }

    // 타이머 시작 (0.01초 간격)
    GetWorld()->GetTimerManager().SetTimer(
        SkillCoolDownTimer,
        [this]()
        {
            // 실제 경과 시간만큼 감소
            float DeltaTime = GetWorld()->GetDeltaSeconds();
            RemainingCoolTime -= DeltaTime;

            // 비율: 0 ~ 1 (1에서 0으로 떨어짐)
            float Percent = FMath::Clamp(RemainingCoolTime / MaxCoolTime, 0.0f, 1.0f);
            SkillCoolTime->SetPercent(Percent);

            // 쿨타임 끝
            if (RemainingCoolTime <= 0.0f)
            {
                RemainingCoolTime = 0.0f;
                SkillCoolTime->SetPercent(0.0f);

                GetWorld()->GetTimerManager().ClearTimer(SkillCoolDownTimer);
            }

        },
        0.01f,
        true
    );
}
