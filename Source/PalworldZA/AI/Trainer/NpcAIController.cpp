 // Fill out your copyright notice in the Description page of Project Settings.


#include "NpcAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NpcBBKeys.h"

ANpcAIController::ANpcAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT
	("/Game/AI/Trainer/BB_NPTrainer.BB_NPTrainer")
	);
	if (BBAssetRef.Succeeded())
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT
	("/Game/AI/Trainer/BT_NPTrainer.BT_NPTrainer")
	);
	if (BTAssetRef.Succeeded())
	{
		BTAsset = BTAssetRef.Object;
	}
}

void ANpcAIController::RunAI()
{
	//사용할 블랙보드 지정
	UBlackboardComponent* BlackboardComponent = Blackboard.Get();

	if (UseBlackboard(BBAsset, BlackboardComponent))
	{
		//NPC의 시작 위치 
		FVector HomeLocation = GetPawn()->GetActorLocation();

		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, HomeLocation);

		bool Result = RunBehaviorTree(BTAsset);
		ensureAlways(Result);
	}
}

void ANpcAIController::StopAI()
{
	// 실행 중인 BT 컴포넌트 받아오기.
	UBehaviorTreeComponent* BTComp
		= Cast<UBehaviorTreeComponent>(BrainComponent);

	if (BTComp)
	{
		BTComp->StopTree();
	}
}

void ANpcAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
