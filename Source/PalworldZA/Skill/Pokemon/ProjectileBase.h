// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PokemonInterface/ProjectileController.h"
#include "ProjectileBase.generated.h"


UCLASS()
class PALWORLDZA_API AProjectileBase : 
	public AActor,
	public IProjectileController
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnProjectileOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// 총알 파괴 
	virtual void DestroyProjectile() override;

	// 총알이 데미지 이벤트로 넘겨줄 정보 저장
	virtual void SetDamageEvent(FPokemonDamageEvent Event) override;

	// 일정 시간후 총알 파괴
	virtual void SetDestroyTimer(float Sec) override;

	// 파괴 이벤트시 호출할 이벤트 등록
	virtual void BindDestroyEvent(const FDestroyMember& InDelegate) override;

protected:

	UPROPERTY()
	TObjectPtr<UShapeComponent> ProjectileCollision;

protected:

	float CurrentDestroyCountTime = 0.0f;
	
	float TargetDestroyCountTime = 0.0f;

	FDestroyMember DestroyEvent;

	FPokemonDamageEvent DamageEvent;
};
