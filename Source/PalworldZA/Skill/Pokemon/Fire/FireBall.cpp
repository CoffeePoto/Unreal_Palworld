// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/Fire/FireBall.h"
#include "Skill/Pokemon/ProjectileBase.h"
#include "Interface/PokemonInterface/ProjectileController.h"
#include "Game/GameSingleton.h"

AFireBall::AFireBall()
{
	static ConstructorHelpers::FClassFinder<AProjectileBase> FireBallRef(TEXT("/Game/BluePrint/PokemonSkill/Fire/BP_FireBallProjectile.BP_FireBallProjectile_C"));

	if (FireBallRef.Succeeded())
	{
		FireBallClass = FireBallRef.Class;
	}
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
	
	IProjectileController* PController = Cast<IProjectileController>(FireBall);

	PController->SetDamageEvent(MakeDamageEvent());
	PController->SetDestroyTimer(2.0f);

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

AProjectileBase* AFireBall::SpawnProjectile(FVector Pos, FRotator Rot)
{
	FActorSpawnParameters SpawnParams;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(
		FireBallClass,
		Pos,
		Rot,
		SpawnParams
	);

	return Projectile;
}
