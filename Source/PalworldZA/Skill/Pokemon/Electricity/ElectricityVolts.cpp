// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill/Pokemon/Electricity/ElectricityVolts.h"
#include "Skill/Pokemon/ProjectileBase.h"
#include "Interface/PokemonInterface/ProjectileController.h"
#include "Game/GameSingleton.h"

AElectricityVolts::AElectricityVolts()
{
	static ConstructorHelpers::FClassFinder<AActor> ElectricityVoltsRef(TEXT("/Game/BluePrint/PokemonSkill/Electricity/BP_ElectricityVoltsProjectile.BP_ElectricityVoltsProjectile_C"));
	if (ElectricityVoltsRef.Succeeded())
	{
		ElectricityVoltsClass = ElectricityVoltsRef.Class;
	}

	Data.ActionType = EActionType::RANGE;
}

void AElectricityVolts::ExecuteSkill()
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
	ElectricityVolts = SpawnProjectile(UserPos, Rotation);

	IProjectileController* PController = Cast<IProjectileController>(ElectricityVolts);

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

AProjectileBase* AElectricityVolts::SpawnProjectile(FVector Pos, FRotator Rot)
{
	FActorSpawnParameters SpawnParams;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(
		ElectricityVoltsClass,
		Pos,
		Rot,
		SpawnParams
	);

	return Projectile;
}