// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Trainer/BTDecorator_TrainerInRange.h"
#include "NpcBBKeys.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/TrainerInterface/NPTrainerAIInterface.h"

UBTDecorator_TrainerInRange::UBTDecorator_TrainerInRange()
{
	NodeName = TEXT("TrainerInRange");
}

bool UBTDecorator_TrainerInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		return false;
	}

	INPTrainerAIInterface* AIPawn = Cast<INPTrainerAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return false;
	}

	APawn* TargetPokemon = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGETPOKEMON));
	if (!TargetPokemon)
	{
		return false;
	}

	float DistacnceToTarget = ControllingPawn->GetDistanceTo(TargetPokemon);
	float AttackRangeWithRadius = AIPawn->GetAIPatrolRadius();
	bResult = DistacnceToTarget <= AttackRangeWithRadius;
	return bResult;
}
