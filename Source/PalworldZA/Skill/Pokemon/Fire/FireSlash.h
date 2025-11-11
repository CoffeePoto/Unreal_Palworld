// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Pokemon/SkillBase.h"
#include "FireSlash.generated.h"


UCLASS()
class PALWORLDZA_API AFireSlash : public ASkillBase
{
	GENERATED_BODY()

public:
	AFireSlash();

	virtual void PostInitializeComponents() override;

	
	virtual void BeginPlay();

	// 스킬 실행
	virtual void ExecuteSkill() override;

protected:

	UFUNCTION()
	void OnSkillOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// 콜리전 활성화 
	UFUNCTION()
	void ActivateCollision();

protected:

	UPROPERTY()
	TObjectPtr<class USphereComponent> HitCollision;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> AttackTimeline;
};
