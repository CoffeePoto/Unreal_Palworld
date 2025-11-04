// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonBase.h"
#include "AI/Pokemon/PokemonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Animation/Pokemon/PokemonAnimInstanceBase.h"


APokemonBase::APokemonBase()
{
	//PrimaryActorTick.bCanEverTick = true;

	// 포켓몬 AI 컨트롤러 생성
	AIControllerClass = APokemonAIController::StaticClass();
	
	// 모두 AIController에 빙의되도록 설정.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FClassFinder<UAnimInstance> PokemonAnimRef(TEXT("/Game/Animation/Pokemon/ABP_PokemonBase.ABP_PokemonBase_C"));
	if (PokemonAnimRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(PokemonAnimRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UPokemonAnimSequenceData> AnimSequenceRef(TEXT("/Game/Data/Pokemon/DA_DarkCrowAnimSequence.DA_DarkCrowAnimSequence"));
	if (AnimSequenceRef.Succeeded())
	{
		AnimData = AnimSequenceRef.Object;
	}
}

void APokemonBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APokemonBase::BeginPlay()
{
	Super::BeginPlay();

	UPokemonAnimInstanceBase* animInstance = 
		Cast<UPokemonAnimInstanceBase>(GetMesh()->GetAnimInstance());

	// 애니메이션 시퀀스 전달
	if (AnimData)
	{
		animInstance->SetAnimSequence(AnimData);
	}
	
	// 블랙 보드 연결
	BBComponent = Cast<APokemonAIController>(GetController())->GetBlackboardComponent();		
}

void APokemonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SkillCoolDown(DeltaTime);
}

void APokemonBase::SkillCoolDown(float DeltaTime)
{
	for (TPair<FName, float>& Skill : SkillCoolTimes)
	{
		if (Skill.Value > 0.0f)
		{
			Skill.Value = FMath::Max(Skill.Value - DeltaTime, 0.0f);
		}
	}
}

float APokemonBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

void APokemonBase::UsingSkill(int SkillNumber)
{
	UE_LOG(LogTemp, Log, TEXT("포켓몬이 트레이너의 지시를 듣고 있습니다."));
}

void APokemonBase::SetActive(FVector Location)
{
	// 지정 위치로 이동
	SetActorLocation(Location);

	// 캐릭터 가시화
	SetActorHiddenInGame(false);

	// 충돌 활성화
	SetActorEnableCollision(true);

	// 틱 활성화 
	SetActorTickEnabled(true);

	// AI 활성 
	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		UBrainComponent* Brain = AI->GetBrainComponent();

		if (Brain)
		{
			Brain->RestartLogic();
		}
	}
}

void APokemonBase::Deactive()
{
	// 캐릭터 비가시화
	SetActorHiddenInGame(true);

	// 충돌 비활성화
	SetActorEnableCollision(false);

	// 틱 비활성화 
	SetActorTickEnabled(false);

	// AI 비활성
	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		UBrainComponent* Brain = AI->GetBrainComponent();

		if (Brain)
		{
			Brain->StopLogic(TEXT("Pokemon Deactive"));
		}
	}
}

void APokemonBase::BindOnPokemonDown(const FOnPokemonDown& InDelegate)
{
	PokemonDownEvents = InDelegate;
}

void APokemonBase::EndSkill()
{
	bIsOnSkill = false;
}


// Called to bind functionality to input
//void APokemonBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//}

