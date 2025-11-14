// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/Electricity/ElectricityBall.h"

AElectricityBall::AElectricityBall()
{
	static ConstructorHelpers::FClassFinder<AActor> ElectricityBallRef(TEXT("/Game/BluePrint/PokemonSkill/BP_ElectricityBallProjectile.BP_ElectricityBallProjectile_C"));
	if (ElectricityBallRef.Succeeded())
	{
		ElectricityBallClass = ElectricityBallRef.Class;
	}

	Data.ActionType = EActionType::RANGE;
}

void AElectricityBall::ExecuteSkill()
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
	ElectricityBall = SpawnProjectile(UserPos, Rotation);
	ElectricityBall->SetLifeSpan(2.0f);

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

AActor* AElectricityBall::SpawnProjectile(FVector Pos, FRotator Rot)
{
	FActorSpawnParameters SpawnParams;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* Projectile = GetWorld()->SpawnActor<AActor>(
		ElectricityBallClass,
		Pos,
		Rot,
		SpawnParams
	);

	return Projectile;
}