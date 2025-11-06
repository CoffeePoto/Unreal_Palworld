// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/TestCloseRangeSkill.h"
#include "TestCloseRangeSkill.h"

ATestCloseRangeSkill::ATestCloseRangeSkill()
{
	static ConstructorHelpers::FClassFinder<AActor> FireSlashRef(TEXT("/Game/BluePrint/TestPokemon/BP_TestFireSlash.BP_TestFireSlash_C"));
	if (FireSlashRef.Succeeded())
	{
		FireSlash = FireSlashRef.Class;
	}
}

void ATestCloseRangeSkill::ExecuteSkill()
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

	// 타겟 앞까지 돌진할 위치 계산
	StopDistance = 100.0f;
	FVector RushEndLocation = TargetPos - (Direction * StopDistance);

	// 돌진 속도
	RushSpeed = 1000.0f;
	float RushDuration = DistanceToTarget / RushSpeed;

	// 실제 돌진 이동 시작.
	
	// 스킬 소환.
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* SpawnedEffect = GetWorld()->SpawnActor<AActor>(
		FireSlash,
		UserPos,
		LookAtRotation,
		SpawnParams
	);
	
	
}
