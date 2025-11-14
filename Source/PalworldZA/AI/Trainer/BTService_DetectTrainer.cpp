// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Trainer/BTService_DetectTrainer.h"
#include "NpcBBKeys.h"
#include "AIController.h"
#include "Interface/TrainerInterface/NPTrainerAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h" // 추가
#include "DrawDebugHelpers.h"

UBTService_DetectTrainer::UBTService_DetectTrainer()
{
	NodeName = TEXT("Detect Trainer");
	Interval = 1.0f;
}

void UBTService_DetectTrainer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	//플레이어 탐지 로직 구현

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector PawnLocation = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	INPTrainerAIInterface* AIPawn = Cast<INPTrainerAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}	

	float DetectRange = AIPawn->GetAIDetectRange();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		PawnLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(DetectRange),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				// 기존에 Target을 설정하던 부분을 MyPokemon으로 변경
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_MYPOKEMON, Pawn);
				DrawDebugSphere(World, PawnLocation, DetectRange, 16, FColor::Green, false, 0.2f); // PawnLocation / DetectRange 사용

				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.2f);
				return;
			}
		}
	}

	// 인식 실패 시 nullptr로 초기화
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_MYPOKEMON, nullptr);
	DrawDebugSphere(World, PawnLocation, DetectRange, 16, FColor::Red, false, 0.2f);	
}
