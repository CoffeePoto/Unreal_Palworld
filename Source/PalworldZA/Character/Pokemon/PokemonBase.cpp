// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonBase.h"
#include "AI/Pokemon/PokemonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Animation/Pokemon/PokemonAnimInstanceBase.h"
#include "Interface/PokemonInterface/PokemonSkill.h"
#include "Skill/Pokemon/SkillBase.h"


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
	if (AnimData) { animInstance->SetAnimSequence(AnimData); }
	
	// 블랙 보드 연결
	BBComponent = Cast<APokemonAIController>(GetController())->GetBlackboardComponent();		
}

void APokemonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SkillCoolDown(DeltaTime);
	UpdateSkillTarget();
}

void APokemonBase::SkillCoolDown(float DeltaTime)
{
	for (FSkillContainer& Skill : PokemonSkills)
	{
		if (Skill.CoolDown > 0.0f)
		{
			Skill.CoolDown = FMath::Max(Skill.CoolDown - DeltaTime, 0.0f);
		}
	}
}

void APokemonBase::UpdateSkillTarget()
{
	if (ActionState != EPokemonAction::NonCommand) { return; }
	if (CurrentSkillTarget == NewSkillTarget) { return; }

	CurrentSkillTarget = NewSkillTarget;
}

void APokemonBase::LoadAnimSequenceData(FString Path)
{
	AnimData = LoadObject<UPokemonAnimSequenceData>(nullptr, *Path);
}

ASkillBase* APokemonBase::SpawnSkill(int SkillIndex)
{
	UClass* SkillType = PokemonSkills[SkillIndex].Skill.Get();

	FActorSpawnParameters SpawnParams;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ASkillBase* SpawnSk = GetWorld()->SpawnActor<ASkillBase>(
		SkillType,
		GetActorLocation(),
		GetActorRotation(),
		SpawnParams
	);

	return SpawnSk;
}

void APokemonBase::ExecuteSkill()
{
//	ASkillBase* SpawnSk = SpawnSkill(SkillNumber);
//	if (!SpawnSk) { return false; }
//
//	IPokemonSkill* SkillController = Cast<IPokemonSkill>(SpawnSk);
//
//	SpawnSk->InitializeSkill(this);
//	ActionState = EPokemonAction::OnSkill;
//
//	SkillController->ExecuteSkill();
}

float APokemonBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

bool APokemonBase::UsingSkill(uint8 SkillNumber)
{
	if (ActionState != EPokemonAction::NonCommand) { return false; }
	if (!PokemonSkills[SkillNumber].Skill) { return false; }
	
	SelectSkillNumber = SkillNumber;
	ActionState = EPokemonAction::InCommand;


	ASkillBase* SpawnSk = SpawnSkill(SkillNumber);
	if (!SpawnSk) { return false; }

	IPokemonSkill* SkillController = Cast<IPokemonSkill>(SpawnSk);

	SpawnSk->InitializeSkill(this);
	ActionState = EPokemonAction::OnSkill;

	SkillController->ExecuteSkill();

	return true;
}

bool APokemonBase::SetActive(FVector Location)
{
	if (ActionState != EPokemonAction::NonCommand) { return false; }

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

	return true;
}

bool APokemonBase::Deactive()
{
	if (ActionState != EPokemonAction::NonCommand) { return false; }

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

	return true;
}

void APokemonBase::BindOnPokemonDown(const FOnPokemonDown& InDelegate)
{
	PokemonDownEvents = InDelegate;
}

void APokemonBase::SetTarget(AActor* NewTarget)
{
	NewSkillTarget = NewTarget;
}

void APokemonBase::EndSkill()
{
	ActionState = EPokemonAction::NonCommand;
}


// Called to bind functionality to input
//void APokemonBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//}

