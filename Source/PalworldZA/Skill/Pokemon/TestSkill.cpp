// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/TestSkill.h"
#include "Interface/PokemonInterface/PokemonDataGetter.h"

ATestSkill::ATestSkill()
{
	static ConstructorHelpers::FClassFinder<AActor> FireBallRef(TEXT("/Game/BluePrint/TestPokemon/BP_TestFire.BP_TestFire_C"));
	if (FireBallRef.Succeeded())
	{
		FireBall = FireBallRef.Class;
	}
}

void ATestSkill::ExecuteSkill()
{
	// User Null 체크
	if (!User) { return; }

	// User, Target 불러오기
	IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(User);
	AActor* Target = Getter->GetTarget();
	
	// Target Null 체크
	if (!Target) 
	{
		UE_LOG(LogTemp, Log, TEXT("없음"));
		return; 
	}
	// Target 위치 가져오기
	FVector Pos = Target->GetActorLocation();
	Pos.Z += 10;

	// User가 Target을 바라보는 방향 단위 벡터
	FVector LookRot = Pos - User->GetActorLocation();
	LookRot.Normalize();
	FRotator Rot = LookRot.Rotation();

	FActorSpawnParameters SpawnParams;
	// 투사체 사용을 위한 Parameter
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 스킬 소환
	AActor* SpawnSk = GetWorld()->SpawnActor<AActor>(
		FireBall,
		User->GetActorLocation(),
		Rot,
		SpawnParams
	);

	// 스킬 Null 체크.
	if (!SpawnSk)
	{
		UE_LOG(LogTemp, Log, TEXT("실패"));
		return;
	}
	SpawnSk->SetLifeSpan(2.0f);

	FTimerHandle SkillEndTimer;

	// CPP
	GetWorldTimerManager().SetTimer(
		SkillEndTimer,
		this,
		&ASkillBase::OnEndSkill,  // 2초 뒤 실행할 함수
		2.0f,                     // 2초 뒤
		false                     // 반복 여부 (false = 한 번만)
	);

	UE_LOG(LogTemp, Log, TEXT("테스트 스킬"));
}