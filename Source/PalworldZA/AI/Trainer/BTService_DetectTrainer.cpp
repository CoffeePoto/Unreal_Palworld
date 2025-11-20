// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Trainer/BTService_DetectTrainer.h"
#include "NpcBBKeys.h"
#include "AIController.h"
#include "Interface/TrainerInterface/NPTrainerAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"
#include "Interface/PokemonInterface/CommandReceiver.h"
#include "character/Pokemon/PokemonBase.h"

// 이 서비스의 역할:
// - 일정 주기(Interval)마다 NPC 트레이너 주변을 탐지해서
// - 감지 범위 안에 있는 "적 포켓몬"을 찾아 블랙보드에 설정한다.
// - 자기 자신의 포켓몬(MyPokemon)은 타겟 후보에서 제외한다.
UBTService_DetectTrainer::UBTService_DetectTrainer()
{
	// 비헤이비어 트리 에디터에서 이 노드에 표시될 이름
	NodeName = TEXT("Detect Trainer");

	// TickNode가 호출되는 주기(초 단위)
	Interval = 1.0f;
}

void UBTService_DetectTrainer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 1. 이 비헤이비어 트리를 실행 중인 AIController 가져오기
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return;
	}

	// 2. AIController가 조종하는 Pawn (NPC 트레이너) 가져오기
	APawn* ControllingPawn = AIController->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return;
	}

	// 3. 현재 위치, 월드 포인터 가져오기
	FVector PawnLocation = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (World == nullptr)
	{
		return;
	}

	// 4. 트레이너 AI 인터페이스로 캐스팅해서 AI 전용 데이터(탐지 범위 등) 사용
	INPTrainerAIInterface* AIPawn = Cast<INPTrainerAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return;
	}

	// 5. 감지 범위 가져오기 (인터페이스에서 제공)
	float DetectRange = AIPawn->GetAIDetectRange();

	// 6. 블랙보드에서 "내 포켓몬(MyPokemon)" 가져오기
	//    - 이 값은 다른 곳(스폰 시점 등)에서 BBKEY_MYPOKEMON 키로 세팅되어 있다고 가정
	APokemonBase* MyPokemon = Cast<APokemonBase>(
		OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_MYPOKEMON)
	);

	// 7. 오버랩(Overlap)을 이용해 탐지 범위 안의 Pawn들을 모두 검사
	TArray<FOverlapResult> OverlapResults;

	// 충돌 쿼리 설정
	// - 마지막 인자 ControllingPawn: 자기 자신은 무시하도록 설정
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);

	// 구형(Sphere) 범위를 이용해 Pawn 채널(ECC_Pawn)과 겹치는 액터들을 찾는다
	bool bOverlapHit = World->OverlapMultiByChannel(
		OverlapResults,                  // 결과 배열
		PawnLocation,                    // 중심 위치
		FQuat::Identity,                 // 회전(필요 없음)
		ECollisionChannel::ECC_Pawn,     // Pawn 채널만 탐색
		FCollisionShape::MakeSphere(DetectRange), // 감지 반경
		CollisionQueryParam              // 쿼리 파라미터
	);

	// 이번 Tick에서 적 포켓몬을 찾았는지 여부
	bool bFoundPokemon = false;

	if (bOverlapHit)
	{
		// 8. 오버랩 결과를 하나씩 검사
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			// 오버랩된 액터를 Pawn으로 캐스팅
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn == nullptr)
			{
				continue;
			}

			// 자기 자신(NPC 트레이너 본인)은 스킵
			if (Pawn == ControllingPawn)
			{
				continue;
			}

			// 포켓몬 베이스 클래스로 캐스팅
			APokemonBase* Pokemon = Cast<APokemonBase>(Pawn);
			if (Pokemon == nullptr)
			{
				// 포켓몬이 아니면 스킵 (플레이어, 다른 NPC 등)
				continue;
			}

			// 9. 내 포켓몬(MyPokemon)과 같은 개체라면 "적"이 아니므로 스킵
			if (Pokemon == MyPokemon)
			{
				continue;
			}

			// 필요하다면 여기에서 "플레이어 소유 포켓몬인지", "팀 구분" 등의 추가 필터링도 가능
			// 예:
			// if (!Pokemon->IsPlayerPokemon())
			// {
			//     continue;
			// }

			// 10. 여기까지 통과했다면 "적 포켓몬"으로 간주
			//     블랙보드에 타겟 포켓몬으로 설정
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGETPOKEMON, Pokemon);
			ICommandReceiver* Controller = Cast<ICommandReceiver>(MyPokemon);
			if (Controller) { Controller->SetTarget(Pokemon); }

			

			// 11. 디버그 표시 (초록색: 적 포켓몬을 성공적으로 탐지)
			DrawDebugSphere(World, PawnLocation, DetectRange, 16, FColor::Green, false, 0.2f);
			DrawDebugPoint(World, Pokemon->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
			DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pokemon->GetActorLocation(), FColor::Green, false, 0.2f);

			bFoundPokemon = true;
			break; // 첫 번째로 찾은 적 포켓몬 하나만 사용
		}
	}

	// 12. 이번 Tick에서 적 포켓몬을 하나도 찾지 못한 경우
	if (!bFoundPokemon)
	{
		// 블랙보드의 타겟 포켓몬 값을 비워서
		// "현재 공격할 대상이 없다"는 상태로 만든다
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGETPOKEMON, nullptr);

		// 디버그 표시 (빨간색: 범위 안에 적 포켓몬 없음)
		DrawDebugSphere(World, PawnLocation, DetectRange, 16, FColor::Red, false, 0.2f);
	}
}
