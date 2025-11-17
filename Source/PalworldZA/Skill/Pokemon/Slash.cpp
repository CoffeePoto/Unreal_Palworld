// Fill out your copyright notice in the Description page of Project Settings.


#include "Slash.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "Game/GameSingleton.h"

ASlash::ASlash()
{
	PrimaryActorTick.bCanEverTick = true;
	AttackTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AttackTimeline"));
}

void ASlash::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// 타임 라인 생성
	HitCollision = Cast<USphereComponent>(SearchCollision(TEXT("HitBox")));
	if (HitCollision)
	{
		HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HitCollision->OnComponentBeginOverlap.AddDynamic(this, &ASlash::OnSkillOverlap);
	}
}

void ASlash::BeginPlay()
{
	Super::BeginPlay();

	if (AttackTimeline)
	{
		// 타임 라인 기간 설정
		AttackTimeline->SetTimelineLength(2.0f);
		AttackTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);

		// 타임라인 델리게이트
		FOnTimelineEvent E1, E2;

		// 델리게이트 바인드
		E1.BindUFunction(this, FName("ActivateCollision"));
		E2.BindUFunction(this, FName("OnEndSkill"));

		// 구간에 함수 배치
		AttackTimeline->AddEvent(0.2f, E1);
		AttackTimeline->AddEvent(2.0f, E2);
	}
}

void ASlash::ExecuteSkill()
{
	// User Null 체크
	if (!User) { return; }
	IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(User);

	// Target Null 체크
	AActor* Target = Getter->GetTarget();
	if (!Target)
	{
		UE_LOG(LogTemp, Log, TEXT("없음"));
		return;
	}

	// 스킬 타임라인 시작
	AttackTimeline->PlayFromStart();
}

void ASlash::OnSkillOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) { return; }

	UE_LOG(LogTemp, Log, TEXT("전달한 데미지: %f"), DamageCalculator());
	
	// 충돌 물체에 데이터 전달 
	OtherActor->TakeDamage(
		DamageCalculator(),          
		MakeDamageEvent(),           
		GetInstigatorController(),    
		this                          
	);
}

void ASlash::ActivateCollision()
{
	if (HitCollision)
	{
		HitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

