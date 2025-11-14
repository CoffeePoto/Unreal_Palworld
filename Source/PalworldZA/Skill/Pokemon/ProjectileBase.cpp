// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileCollision = FindComponentByClass<UShapeComponent>();

	if (ProjectileCollision)
	{
		ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnProjectileOverlap);
	}
}

void AProjectileBase::DestroyProjectile()
{
	// 중복 파괴 방지
	if (IsPendingKillPending() || IsActorBeingDestroyed())
	{
		return;
	}

	DestroyEvent.ExecuteIfBound();
	Destroy();
}

void AProjectileBase::SetDamageEvent(FPokemonDamageEvent Event)
{
	DamageEvent = Event;
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AProjectileBase::SetDestroyTimer(float Sec)
{
	TargetDestroyCountTime = Sec;
	CurrentDestroyCountTime = 0.0f;
}

void AProjectileBase::BindDestroyEvent(const FDestroyMember& InDelegate)
{
	DestroyEvent = InDelegate;
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetDestroyCountTime > 0.0f)
	{
		CurrentDestroyCountTime += DeltaTime;

		if (CurrentDestroyCountTime >= TargetDestroyCountTime)
		{
			DestroyProjectile();
		}
	}
}

void AProjectileBase::OnProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DamageEvent.Pokemon)
	{
		UE_LOG(LogTemp, Log, TEXT("전달한 데미지: %f"), DamageEvent.Power);
		

		// 충돌 물체에 데이터 전달 
		OtherActor->TakeDamage(
			DamageEvent.Power,
			DamageEvent,
			GetInstigatorController(),
			this
		);
	}
}

