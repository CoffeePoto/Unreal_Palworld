// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Pokemon/BTTask_PokemonSkill.h"
#include "Interface/PokemonInterface/CommandReceiver.h"
#include "Interface/PokemonInterface/HardCommandReceiver.h"
#include "AIController.h"

UBTTask_PokemonSkill::UBTTask_PokemonSkill()
{
}

EBTNodeResult::Type UBTTask_PokemonSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 컨트롤 폰 로드
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn) { return EBTNodeResult::Failed; }

	// 명령 인터페이스 로드
	ICommandReceiver* Controller = Cast<ICommandReceiver>(ControllingPawn);
	if (!Controller) { return EBTNodeResult::Failed; }

	// 강한 명령 인터페이스 로드 
	IHardCommandReceiver* HardController = Cast<IHardCommandReceiver>(ControllingPawn);
	if (!HardController) { return EBTNodeResult::Failed; }

	// 델리게이트 등록
	if (!SkillEndEventHandle.IsValid())
	{
		OwnerComponent = &OwnerComp;

		SkillEndEventHandle = Controller->BindEndPokemonSkill(
			FEndPokemonSkill::FDelegate::CreateUObject(this, &UBTTask_PokemonSkill::FinishSkillTask)
		);
	}
	
	HardController->ExecuteSkill();

	return EBTNodeResult::InProgress;
}

void UBTTask_PokemonSkill::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();	

	if (ICommandReceiver* Controller = Cast<ICommandReceiver>(ControllingPawn))
	{
		if (SkillEndEventHandle.IsValid())
		{
			Controller->UnBindEndPokemonSkill(SkillEndEventHandle);
			SkillEndEventHandle.Reset();
		}
	}

	OwnerComponent = nullptr;

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_PokemonSkill::FinishSkillTask()
{
	if (OwnerComponent)
	{
		FinishLatentTask(*OwnerComponent, EBTNodeResult::Succeeded);
	}
}
