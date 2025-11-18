// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Pokemon/SkillBase.h"
#include "FireTornado.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API AFireTornado : public ASkillBase
{
	GENERATED_BODY()
public:
	AFireTornado();

	virtual void PostInitializeComponents() override;


	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// 스킬 실행
	virtual void ExecuteSkill() override;

	// 스킬 종료
	virtual void OnEndSkill() override;

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

	TObjectPtr<class UCapsuleComponent> HitCollision;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> AttackTimeline;

private:
	
	

	const FVector START_SIZE = FVector(1.0f, 1.0f, 40.0f);
	const FVector MAX_SIZE = FVector(40.0f, 40.0f, 40.0f);
	FVector CurrentSize;

	float InterpSpeed = 1.0f;
	
};
