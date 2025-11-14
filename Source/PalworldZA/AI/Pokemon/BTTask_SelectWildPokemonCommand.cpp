// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Pokemon/BTTask_SelectWildPokemonCommand.h"
#include "Interface/PokemonInterface/CommandReceiver.h"
#include "Interface/PokemonInterface/PokemonDataGetter.h"
#include "Interface/PokemonInterface/HardCommandReceiver.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_SelectWildPokemonCommand::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn) { return EBTNodeResult::Failed; }

	// 명령 인터페이스 로드
	ICommandReceiver* Controller = Cast<ICommandReceiver>(ControllingPawn);
	if (!Controller) { return EBTNodeResult::Failed; }

	IHardCommandReceiver* HardController = Cast<IHardCommandReceiver>(ControllingPawn);
	if (!HardController) { return EBTNodeResult::Failed; }

	IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(ControllingPawn);
	if (!Getter) { return EBTNodeResult::Failed; }

	TArray<FCurrentPokemonSkillData> Skills;
	Getter->GetSkillData(Skills);

	int32 RandomSkillNumber = FMath::RandRange(0, Skills.Num() - 1);
	HardController->ReservationSkill(RandomSkillNumber);

	return EBTNodeResult::Type();
}
