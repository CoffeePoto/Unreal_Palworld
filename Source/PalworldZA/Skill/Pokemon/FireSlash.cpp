// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/FireSlash.h"
#include "FireSlash.h"

AFireSlash::AFireSlash()
{
	Data.ActionType = EActionType::MELEE;
}

void AFireSlash::ExecuteSkill()
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

	// Target, User 위치 가져오기.
	FVector TargetPos = Target->GetActorLocation();
	FVector UserPos = User->GetActorLocation();

	// 거리 구하기.
	FVector Direction = (TargetPos - UserPos);
	float DistanceToTarget = Direction.Size();

	// 타겟 방향으로 회전.
	Direction.Normalize();
	FRotator LookAtRotation = Direction.Rotation();
	User->SetActorRotation(LookAtRotation);

	// 동작 회복
	FTimerHandle SkillEndTimer;
	GetWorldTimerManager().SetTimer(
		SkillEndTimer,
		this,
		&ASkillBase::OnEndSkill,
		0.6f,
		false
	);
}
