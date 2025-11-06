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
	if (!User) { return; }
	IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(User);
	AActor* Target = Getter->GetTarget();
	
	if (!Target) 
	{
		UE_LOG(LogTemp, Log, TEXT("없음"));
		return; 
	}
	FVector Pos = Target->GetActorLocation();
	Pos.Z += 10;

	FVector LookRot = Pos - User->GetActorLocation();
	LookRot.Normalize();
	FRotator Rot = LookRot.Rotation();

	FActorSpawnParameters SpawnParams;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnSk = GetWorld()->SpawnActor<AActor>(
		FireBall,
		User->GetActorLocation(),
		Rot,
		SpawnParams
	);
	
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