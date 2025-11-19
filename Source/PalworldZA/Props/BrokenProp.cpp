// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/BrokenProp.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Field/FieldSystemObjects.h"
#include "Field/FieldSystemNodes.h"

void ABrokenProp::BeginPlay()
{
	Super::BeginPlay();
	GeometryComponent = FindComponentByClass<UGeometryCollectionComponent>();
	GeometryComponent->SetSimulatePhysics(true);

	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		UBrainComponent* Brain = AI->GetBrainComponent();

		if (Brain)
		{
			Brain->StopLogic(TEXT("This is Props"));
		}
	}
}

float ABrokenProp::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (ActionState == EPokemonAction::Down) { return 0.0f; }
	
	if (GeometryComponent)
	{
		GeometryComponent->SetSimulatePhysics(true);

		FVector Center = GeometryComponent->GetComponentLocation();

		// 1) 파괴 조건 생성 (Strain)
		GeometryComponent->ApplyExternalStrain(
			GeometryComponent->GetRootIndex(),
			Center,
			300.0f,
			3,
			1.0f,
			1000000.0f
		);

		GeometryComponent->AddImpulseAtLocation(
			FVector(0, 0, 1) * 1000000.0f,  // Upwards blast
			Center
		);

		UE_LOG(LogTemp, Log, TEXT("파괴"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("파괴 무시"));	
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABrokenProp::PokemonDown()
{
	FTimerHandle DeactiveTimer;
	ActionState = EPokemonAction::Down;

	// 타이머
	GetWorldTimerManager().SetTimer(
		DeactiveTimer,
		this,
		&ABrokenProp::PokemonDownEventFunc,
		10.0f,
		false
	);
}
