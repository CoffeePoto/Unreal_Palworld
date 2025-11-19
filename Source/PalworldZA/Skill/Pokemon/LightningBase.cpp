// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/LightningBase.h"
#include "Skill/Pokemon/ProjectileBase.h"
#include "Interface/PokemonInterface/ProjectileController.h"
#include "LightningBase.h"

ALightningBase::ALightningBase()
{
	// 이펙트 로드.
	static ConstructorHelpers::FClassFinder<AProjectileBase> LightningRef(TEXT(
		"/Game/BluePrint/PokemonSkill/Electricity/BP_ElectricityLightningProjectile.BP_ElectricityLightningProjectile_C"
	));

	if (LightningRef.Succeeded())
	{
		LightningClass = LightningRef.Class;
	}
}

void ALightningBase::ExecuteSkill()
{
	// User Getter.
	if (!User) { return; }
	IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(User);

	// Target Getter.
	AActor* Target = Getter->GetTarget();
	if (!Target) { return; }

	// Target 위치 값 추출
	FVector TargetPos = Target->GetActorLocation();

	// Target 위치에 발사체 생성
	Lightning = SpawnProjectile(TargetPos, FRotator::ZeroRotator);
	
	// 이펙트 컨트롤러 생성.
	IProjectileController* PController = Cast<IProjectileController>(Lightning);

	PController->SetDamageEvent(MakeDamageEvent());
	PController->SetDestroyTimer(2.0f);

	FTimerHandle SkillEndTimer;

	// 타이머
	GetWorldTimerManager().SetTimer(
		SkillEndTimer,
		this,
		&ASkillBase::OnEndSkill,  // 2초 뒤 실행할 함수
		1.0f,                     // 1초 뒤
		false                     // 반복 여부 (false = 한 번만)
	);
}

AProjectileBase* ALightningBase::SpawnProjectile(FVector Pos, FRotator Rot)
{
	FActorSpawnParameters SpawnParams;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(LightningClass, Pos, Rot, SpawnParams);

	return Projectile;
}