// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/Fire/FireBall.h"

AFireBall::AFireBall()
{
	static ConstructorHelpers::FClassFinder<AActor> FireBallRef(TEXT("/Game/BluePrint/PokemonSkill/Fire/BP_FireBallProjectile.BP_FireBallProjectile_C"));

	if (FireBallRef.Succeeded())
	{
		FireBallClass = FireBallRef.Class;
	}
	
	Data.ActionType = EActionType::RANGE;
}

void AFireBall::ExecuteSkill()
{
	if (!User) { return; }
	IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(User);

	AActor* Target = Getter->GetTarget();
	if (!Target) { return; }

	// 위치 값 추출
	FVector UserPos = Getter->GetShootPoint();
	FVector TargetPos = Target->GetActorLocation();

	// 방향 값 생성
	FVector RotationVector = TargetPos - UserPos;
	RotationVector.Normalize();
	FRotator Rotation = RotationVector.Rotation();

	// 발사체 생성 및 생명 주기 설정
	FireBall = SpawnProjectile(UserPos, Rotation); 
	FireBall->SetLifeSpan(2.0f);

	FTimerHandle SkillEndTimer;

	// 타이머
	GetWorldTimerManager().SetTimer(
		SkillEndTimer,
		this,
		&ASkillBase::OnEndSkill,  // 2초 뒤 실행할 함수
		2.0f,                     // 2초 뒤
		false                     // 반복 여부 (false = 한 번만)
	);
}

AActor* AFireBall::SpawnProjectile(FVector Pos, FRotator Rot)
{
	FActorSpawnParameters SpawnParams;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* Projectile = GetWorld()->SpawnActor<AActor>(
		FireBallClass,
		Pos,
		Rot,
		SpawnParams
	);

	return Projectile;
}
