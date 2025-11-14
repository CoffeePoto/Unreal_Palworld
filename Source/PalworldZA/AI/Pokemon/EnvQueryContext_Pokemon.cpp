// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Pokemon/EnvQueryContext_Pokemon.h"
#include "Interface/PokemonInterface/PokemonDataGetter.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "AIController.h"


void UEnvQueryContext_Pokemon::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	UObject* OwnerObj = QueryInstance.Owner.Get();
	if (!OwnerObj) { return; }

	APawn* Requester = Cast<APawn>(OwnerObj);

	// Owner가 AIController인 경우도 대비
	if (!Requester)
	{
		AAIController* AICon = Cast<AAIController>(OwnerObj);
		if (AICon)
		{
			Requester = AICon->GetPawn();
		}
	}

	if (!Requester) { return; }

	IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(Requester);
	if (!Getter) { return; }

	AActor* Target = Getter->GetTarget();
	if (!Target) { return; }


	UEnvQueryItemType_Point::SetContextHelper(ContextData, Target->GetActorLocation());
}
