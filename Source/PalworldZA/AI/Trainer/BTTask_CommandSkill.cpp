// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Trainer/BTTask_CommandSkill.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/TrainerInterface/NPTrainerAIInterface.h"

UBTTask_CommandSkill::UBTTask_CommandSkill()
{
}

EBTNodeResult::Type UBTTask_CommandSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	AIPawn->AICommandSkills();

	return EBTNodeResult::Type();
}
