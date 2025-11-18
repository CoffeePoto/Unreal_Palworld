// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Pokemon/SkillBase.h"
#include "AuraBase.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLDZA_API AAuraBase : public ASkillBase
{
	GENERATED_BODY()

public:
	AAuraBase();

	virtual void PostInitializeComponents() override;


	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// 스킬 실행
	virtual void ExecuteSkill() override;

	virtual void DamageTransfer();

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

	UFUNCTION()
	void OnSkillEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	// 콜리전 활성화 
	UFUNCTION()
	void ActivateCollision();

protected:

	UPROPERTY()
	TObjectPtr<class USphereComponent> HitCollision;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> AttackTimeline;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> OtherArray;
	
};
