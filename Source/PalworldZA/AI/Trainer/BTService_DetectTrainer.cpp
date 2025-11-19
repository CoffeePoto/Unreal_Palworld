// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Trainer/BTService_DetectTrainer.h"
#include "NpcBBKeys.h"
#include "AIController.h"
#include "Interface/TrainerInterface/NPTrainerAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h" // 추가
#include "DrawDebugHelpers.h"
#include "character/Pokemon/PokemonBase.h" 

UBTService_DetectTrainer::UBTService_DetectTrainer()
{
	NodeName = TEXT("Detect Trainer");
	Interval = 1.0f;
}

void UBTService_DetectTrainer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// --- 공통: 컨트롤러 / 월드 / 탐지 범위 가져오기 ---
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return;
	}

	APawn* ControllingPawn = AIController->GetPawn();
	if (ControllingPawn == nullptr)
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

	// --- 오버랩으로 주변 Pawn 검색 ---
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

	// 찾은 포켓몬이 있는지 여부
	bool bFoundPokemon = false;

	// --- 검색된 Pawn 중 플레이어 컨트롤러가 조종하는 Pawn 찾기 ---
	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn == nullptr)
			{
				continue;
			}

			// 자기 자신(이 NPC 트레이너)은 무시
			if (Pawn == ControllingPawn)
			{
				continue;
			}

			// 여기서부터 “포켓몬만” 타겟으로 삼기
			APokemonBase* Pokemon = Cast<APokemonBase>(Pawn);
			if (Pokemon == nullptr)
			{
				continue;
			}

			// (선택) 플레이어가 소환한 포켓몬만 타겟으로 삼고 싶으면 이런 식으로 한 번 더 필터링
			// if (!Pokemon->IsPlayerPokemon())
			// {
			//     continue;
			// }

			// 블랙보드에 포켓몬을 타겟으로 저장
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGETPOKEMON, Pokemon);

			// 디버그 출력: 초록색
			DrawDebugSphere(World, PawnLocation, DetectRange, 16, FColor::Green, false, 0.2f);
			DrawDebugPoint(World, Pokemon->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
			DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pokemon->GetActorLocation(), FColor::Green, false, 0.2f);

			bFoundPokemon = true;
			break; // 첫 번째로 찾은 포켓몬만 사용
		}

	}

	// 포켓몬을 하나도 못 찾은 경우: 타겟 초기화
	if (!bFoundPokemon)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGETPOKEMON, nullptr);
		DrawDebugSphere(World, PawnLocation, DetectRange, 16, FColor::Red, false, 0.2f);
	}
}