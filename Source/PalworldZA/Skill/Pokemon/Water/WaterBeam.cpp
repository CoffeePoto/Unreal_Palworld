// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/Water/WaterBeam.h"
#include "Skill/Pokemon/ProjectileBase.h"
#include "Interface/PokemonInterface/ProjectileController.h"

AWaterBeam::AWaterBeam()
{
	// 이펙트 로드.
	static ConstructorHelpers::FClassFinder<AProjectileBase> WaterBeamRef(TEXT(
		"/Game/BluePrint/PokemonSkill/Water/BP_WaterBeamProjectile.BP_WaterBeamProjectile_C"
	));

	if (WaterBeamRef.Succeeded())
	{
		WaterBeamClass = WaterBeamRef.Class;
	}
}

void AWaterBeam::ExecuteSkill()
{
	// User Getter.
	if (!User) { return; }
	IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(User);

	// Target Getter.
	AActor* Target = Getter->GetTarget();
	if (!Target) { return; }

	// 위치 값 추출
	FVector UserPos = Getter->GetShootPoint();
	FVector TargetPos = Target->GetActorLocation();

	// 방향 값 생성
	FVector RotationVector = UserPos - TargetPos;
	RotationVector.Normalize();
	FRotator Rotation = RotationVector.Rotation();

	// Target 위치에 발사체 생성
	WaterBeam = SpawnProjectile(UserPos, Rotation);

	// 이펙트 컨트롤러 생성.
	IProjectileController* PController = Cast<IProjectileController>(WaterBeam);

	PController->SetDamageEvent(MakeDamageEvent());
	PController->SetDestroyTimer(2.0f);

	FTimerHandle SkillEndTimer;

	// 타이머
	GetWorldTimerManager().SetTimer(
		SkillEndTimer,
		this,
		&ASkillBase::OnEndSkill,  // 2초 뒤 실행할 함수
		2.0f,                     // 1초 뒤
		false                     // 반복 여부 (false = 한 번만)
	);
}

AProjectileBase* AWaterBeam::SpawnProjectile(FVector Pos, FRotator Rot)
{
	FActorSpawnParameters SpawnParams;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(WaterBeamClass, Pos, Rot, SpawnParams);

	return Projectile;
}
