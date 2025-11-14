// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Trainer/BTTask_SummonPokemon.h"
#include "AIController.h"
#include "NpcBBKeys.h"
#include "Interface/TrainerInterface/NPTrainerAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SummonPokemon::UBTTask_SummonPokemon()
{
}

EBTNodeResult::Type UBTTask_SummonPokemon::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, 
	uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//AIPawn È£Ãâ
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	INPTrainerAIInterface* AIPawn = Cast<INPTrainerAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_MYPOKEMON, AIPawn->GetPokemon());
	AIPawn->AISummonPokemon();

	return EBTNodeResult::Type();
}
