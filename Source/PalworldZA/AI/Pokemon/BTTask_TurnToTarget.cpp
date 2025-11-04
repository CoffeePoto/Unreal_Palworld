// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "AIController.h"
#include "PokemonBBKeys.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("TurnToTarget");
	bNotifyTick = true;	
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 회전 폰 정보 불러오기 
	ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn) return EBTNodeResult::Failed;

	// 타겟 폰 정보 불러오기 
	APawn* TargetPawn = Cast<APawn>( OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("EMPTY")) );
	if(!TargetPawn) return EBTNodeResult::Failed;

	// NPC가 바라볼 방향 구하기.
	LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();

	// 회전 값 구하기.
	TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	return EBTNodeResult::InProgress;
}

void UBTTask_TurnToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// 보간 회전 값 생성
	FRotator NewRot = FMath::RInterpTo(
		ControllingPawn->GetActorRotation(),
		TargetRot,
		DeltaSeconds,
		TurnSpeed
	);

	// 보간 회전 값 적용 
	ControllingPawn->SetActorRotation(NewRot);

	float YawDiff = FMath::Abs(FRotator::NormalizeAxis(TargetRot.Yaw - NewRot.Yaw));

	if (YawDiff < 5.0f)
	{
		// 처음 목표 달성 
		ControllingPawn->SetActorRotation(TargetRot);
		// 성공 반환
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
