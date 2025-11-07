// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "PokemonBBKeys.h"

APokemonAIController::APokemonAIController()
{
	// BBAsset
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT(
		"/Game/AI/Pokemon/BB_Pokemon.BB_Pokemon"
	));
	if (BBAssetRef.Succeeded())
	{
		BBAsset = BBAssetRef.Object;
	}

	// BTAsset
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT(
		"/Game/AI/Pokemon/BT_Pokemon.BT_Pokemon"
	));
	if (BTAssetRef.Succeeded())
	{
		BTAsset = BTAssetRef.Object;
	}
}

void APokemonAIController::RunAI()
{
	UBlackboardComponent* BBComponent = Blackboard.Get();

	if (UseBlackboard(BBAsset, BBComponent))
	{
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		// 행동 트리 실행
		bool Result = RunBehaviorTree(BTAsset);

		ensureAlways(Result);
	}
}

void APokemonAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);

	if (BTComponent)
	{
		// 행동 트리 중지
		BTComponent->StopTree();
	}
}

void APokemonAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 빙의 완료후 실행 
	RunAI();

}
