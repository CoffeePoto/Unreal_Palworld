// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/Fire/FireTornado.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "Game/GameSingleton.h"

AFireTornado::AFireTornado()
{
	PrimaryActorTick.bCanEverTick = true;
	AttackTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AttackTimeline"));
}

void AFireTornado::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 타임 라인 생성
	HitCollision = Cast<UCapsuleComponent>(SearchCollision(TEXT("HitBox")));

	if (HitCollision)
	{
		HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HitCollision->OnComponentBeginOverlap.AddDynamic(this, &AFireTornado::OnSkillOverlap);
	}
}

void AFireTornado::BeginPlay()
{
	Super::BeginPlay();

	if (AttackTimeline)
	{
		this->SetActorRotation(FRotator::ZeroRotator);

		// 타임 라인 기간 설정
		AttackTimeline->SetTimelineLength(2.5f);
		AttackTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);

		CurrentSize = START_SIZE;
		HitCollision->SetRelativeScale3D(START_SIZE);

		// 타임라인 델리게이트
		FOnTimelineEvent E1, E2;

		// 델리게이트 바인드
		E1.BindUFunction(this, FName("ActivateCollision"));
		E2.BindUFunction(this, FName("OnEndSkill"));

		// 구간에 함수 배치
		AttackTimeline->AddEvent(0.5f, E1);
		AttackTimeline->AddEvent(2.5f, E2);
	}
}

void AFireTornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentSize.Equals(MAX_SIZE, 0.1f)) { return; }

	// 토네이도 사이즈 조정
	float NewX = FMath::FInterpTo(CurrentSize.X, MAX_SIZE.X, DeltaTime, InterpSpeed);
	float NewY = FMath::FInterpTo(CurrentSize.Y, MAX_SIZE.Y, DeltaTime, InterpSpeed);
	
	CurrentSize = FVector(NewX, NewY, MAX_SIZE.Z);
	HitCollision->SetRelativeScale3D(CurrentSize);
}

void AFireTornado::ExecuteSkill()
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

void AFireTornado::OnEndSkill()
{
	ICommandReceiver* PokemonController = Cast<ICommandReceiver>(User);

	// 버프 설정
	PokemonController->SetBuff(EPokemonBuffStat::ATK, 10.0f);
	PokemonController->SetBuff(EPokemonBuffStat::SPA, 10.0f);
	PokemonController->SetBuff(EPokemonBuffStat::SPEED, 10.0f);

	// 디버프 설정
	PokemonController->SetDeBuff(EPokemonBuffStat::DEF, 10.0f);
	PokemonController->SetDeBuff(EPokemonBuffStat::SPD, 10.0f);

	Super::OnEndSkill();
}

void AFireTornado::OnSkillOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AFireTornado::ActivateCollision()
{
	if (HitCollision)
	{
		HitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}
