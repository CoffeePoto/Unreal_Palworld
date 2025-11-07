// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonBase.h"

#include "AI/Pokemon/PokemonAIController.h"
#include "AI/Pokemon/PokemonBBKeys.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

#include "Animation/Pokemon/PokemonAnimInstanceBase.h"

#include "Interface/PokemonInterface/PokemonSkill.h"
#include "Skill/Pokemon/SkillBase.h"

#include "Data/Pokemon/PokemonDamageEvent.h"


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
	DownRemainingBuffTime(DeltaTime);
	UpdateSkillTarget();
	UpdateBBCommand();
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

void APokemonBase::DownRemainingBuffTime(float DeltaTime)
{
	for (float& Buff : RemainingBuffTimes)
	{
		if (Buff > 0)
		{
			Buff = FMath::Max(Buff - DeltaTime, 0.0f);
		}
	}
}

void APokemonBase::UpdateSkillTarget()
{
	if (ActionState != EPokemonAction::NonCommand) { return; }
	if (CurrentSkillTarget == NewSkillTarget) { return; }

	CurrentSkillTarget = NewSkillTarget;
	BBComponent->SetValueAsObject(BBKEY_TARGET, CurrentSkillTarget);
}

void APokemonBase::UpdateBBCommand()
{
	bool BBComand = BBComponent->GetValueAsBool(BBKEY_IN_COMMAND);
	
	if (ActionState == EPokemonAction::InCommand)
	{
		if (!BBComand)
		{
			BBComponent->SetValueAsBool(BBKEY_IN_COMMAND, true);
		}
	}
	else if (ActionState == EPokemonAction::NonCommand)
	{
		if (BBComand)
		{
			BBComponent->SetValueAsBool(BBKEY_IN_COMMAND, false);
		}
	}
}

void APokemonBase::LoadAnimSequenceData(FString Path)
{
	AnimData = LoadObject<UPokemonAnimSequenceData>(nullptr, *Path);
}

ASkillBase* APokemonBase::SpawnSkill(int SkillIndex)
{
	UClass* SkillType = PokemonSkills[SkillIndex].Skill.Get();

	FActorSpawnParameters SpawnParams;

	// 파라미터 값 세팅
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 스킬 스폰 
	ASkillBase* SpawnSk = GetWorld()->SpawnActor<ASkillBase>(
		SkillType,
		GetActorLocation(),
		GetActorRotation(),
		SpawnParams
	);

	return SpawnSk;
}

void APokemonBase::SetRangeAttackPosition()
{
	if (!Trainer) { return; }

	const FVector Pos = Trainer->GetActorLocation();
	const FVector Forward = Trainer->GetActorForwardVector();
	const FVector Rotated = Forward.RotateAngleAxis(45.0f, FVector::UpVector);
	const FVector Dir = Rotated.GetSafeNormal();
	const FVector SelectPos = Pos + (Dir * 200.0f);

	BBComponent->SetValueAsVector(BBKEY_TARGET_LOCATION, SelectPos);
}

void APokemonBase::ExecuteSkill()
{
	// 스킬 소환
	SpawnedSkill = SpawnSkill(SelectSkillNumber);
	if (!SpawnedSkill) { return; }

	// 스킬 컨트롤러 세팅
	SpawnSkillController.SetObject(SpawnedSkill);
	SpawnSkillController.SetInterface(Cast<IPokemonSkill>(SpawnedSkill));

	// 초기화 및 상태 변환
	SpawnedSkill->InitializeSkill(this);
	ActionState = EPokemonAction::OnSkill;

	// 스킬 실행
	SpawnSkillController->ExecuteSkill();
}

float APokemonBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageEvent.GetTypeID() == FPokemonDamageEvent::ClassID)
	{
		const FPokemonDamageEvent& PokemonDamage = static_cast<const FPokemonDamageEvent&>(DamageEvent);

		// Todo : 데미지 계산 처리
	}
	else
	{
		CurrentStatData.Hp -= DamageAmount;
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

bool APokemonBase::UsingSkill(uint8 SkillNumber)
{
	if (ActionState != EPokemonAction::NonCommand) { return false; }
	if (!PokemonSkills.IsValidIndex(SkillNumber)) { return false; }

	ASkillBase* DefaultSkill = PokemonSkills[SkillNumber].Skill.Get()->GetDefaultObject<ASkillBase>();
	if (!DefaultSkill) { return false; }
	
	SelectSkillNumber = SkillNumber;
	ActionState = EPokemonAction::InCommand;

	FPokemonSkillData SkillData = DefaultSkill->GetSkillData();
	
	BBComponent->SetValueAsEnum(BBKEY_ACTION_TYPE, static_cast<uint8>(SkillData.ActionType));
	if (SkillData.ActionType == EActionType::RANGE)
	{
		SetRangeAttackPosition();
	}

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

FDelegateHandle APokemonBase::BindEndPokemonSkill(const FEndPokemonSkill::FDelegate& InDelegate)
{
	return PokemonSkillEndEvents.Add(InDelegate);
}

void APokemonBase::UnBindEndPokemonSkill(FDelegateHandle Handle)
{
	PokemonSkillEndEvents.Remove(Handle);
}

void APokemonBase::SetTarget(AActor* NewTarget)
{
	NewSkillTarget = NewTarget;
}

void APokemonBase::SetTrainer(APawn* NewTrainer)
{
	Trainer = NewTrainer;
	BBComponent->SetValueAsObject(BBKEY_OWNER, Trainer);
}

void APokemonBase::SetBuff(EPokemonBuffStat Stat, float Time, bool IsCover)
{
	uint8 StatNumber = static_cast<uint8>(Stat);
	if (!RemainingBuffTimes.IsValidIndex(StatNumber)) { return; }

	if (IsCover)
	{
		RemainingBuffTimes[StatNumber] = Time;
	}
	else
	{
		RemainingBuffTimes[StatNumber] += Time;
	}

	if(RemainingBuffTimes[StatNumber] < 0) { RemainingBuffTimes[StatNumber] = 0; }
}

void APokemonBase::EndSkill()
{
	// 스킬 쿨타임 설정
	if (SpawnSkillController)
	{
		PokemonSkills[SelectSkillNumber].CoolDown = SpawnSkillController->GetSkillData().Cooltime;
	}

	// 스폰된 스킬 제거
	if (SpawnedSkill)
	{
		SpawnedSkill->Destroy();
		SpawnSkillController = nullptr;
		SpawnedSkill = nullptr;
	}

	PokemonSkillEndEvents.Broadcast();
	ActionState = EPokemonAction::NonCommand;
}
