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
#include "Data/Pokemon/FPokemonTypeTable.h"


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

	// 버프 타이머 초기화 
	RemainingBuffTimes.SetNum((uint8)EPokemonBuffStat::COUNT);
	for (float& Time : RemainingBuffTimes) { Time = ZERO; }
	
	// 블랙 보드 연결
	BBComponent = Cast<APokemonAIController>(GetController())->GetBlackboardComponent();	
}

void APokemonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SkillCoolDown(DeltaTime);
	DownRemainingBuffTime(DeltaTime);
	DamageCauserQueueDequeue(DeltaTime);
	UpdateSkillTarget();
	UpdateBBCommand();
}

void APokemonBase::SkillCoolDown(float DeltaTime)
{
	for (FSkillContainer& Skill : PokemonSkills)
	{
		if (Skill.CoolDown > ZERO)
		{
			Skill.CoolDown = FMath::Max(Skill.CoolDown - DeltaTime, ZERO);
		}
	}
}

void APokemonBase::DownRemainingBuffTime(float DeltaTime)
{
	for (float& Buff : RemainingBuffTimes)
	{
		if (Buff > 0)
		{
			Buff = FMath::Max(Buff - DeltaTime, ZERO);
		}
	}
}

void APokemonBase::DamageCauserQueueDequeue(float DeltaTime)
{
	// 비워져 있으면 리턴
	if (DamageCauserArray.IsEmpty())
	{
		return;
	}

	// 만약 인덱스가 개수를 넘어가면 초기화 
	if (DamageCauserArray.Num() <= DamageCauserIndex)
	{
		DamageCauserArray.Reset();
		DamageCauserIndex = 0;
		CurrentDCDStartTime = ZERO;
		return;
	}

	// 비우는 시간을 일정 시간 뒤로 미루기 위해 카운트
	if (DAMAGE_CAUSER_DEQUEUE_START_TIME > CurrentDCDStartTime)
	{
		CurrentDCDStartTime += DeltaTime;
		return;
	}

	// 주기적으로 인덱스를 비워줌 
	if (DAMAGE_CAUSER_DEQUEUE_CYCLE_TIME <= CurrentDCDCycleTime)
	{
		DamageCauserArray[DamageCauserIndex] = nullptr;
		++DamageCauserIndex;
		CurrentDCDCycleTime = ZERO;
		return;
	}

	CurrentDCDCycleTime += DeltaTime;
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

const FPokemonStatData APokemonBase::CalculateCurrentStat()
{
	FPokemonStatData ReturnStat;
	
	ReturnStat.Hp = DefaultStatData.Hp;

	ReturnStat.ATK   = CalculateStatParameters(EPokemonBuffStat::ATK, DefaultStatData.ATK);
	ReturnStat.DEF   = CalculateStatParameters(EPokemonBuffStat::DEF, DefaultStatData.DEF);
	ReturnStat.SPA   = CalculateStatParameters(EPokemonBuffStat::SPA, DefaultStatData.SPA);
	ReturnStat.SPD	 = CalculateStatParameters(EPokemonBuffStat::SPD, DefaultStatData.SPD);
	ReturnStat.Speed = CalculateStatParameters(EPokemonBuffStat::SPEED, DefaultStatData.Speed);

	return ReturnStat;
}

float APokemonBase::CalculateStatParameters(EPokemonBuffStat Stat, float DefaultStat)
{
	float Index = static_cast<uint8>(Stat);

	if (RemainingBuffTimes[Index] > 0)
	{
		DefaultStat *= 1.5f;
	}
	
	return DefaultStat;
}

void APokemonBase::PokemonDown()
{
	PokemonDownEvents.Execute();
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
	float FinalDMG = ZERO;

	// 중복 충돌 방지
	if(DamageCauserArray.Contains(DamageCauser)) { return ZERO; }

	if (DamageEvent.GetTypeID() == FPokemonDamageEvent::ClassID)
	{
		const FPokemonDamageEvent& PokemonDamage = static_cast<const FPokemonDamageEvent&>(DamageEvent);

		if (Trainer == PokemonDamage.Trainer) { return ZERO; }
		if (Cast<APawn>(this) == PokemonDamage.Pokemon) { return ZERO; }

		float DefenseStat = ZERO;
		float MulType1 = FPokemonTypeTable::GetMultiplier(PokemonDamage.Type, DefaultStatData.Type1);
		float MulType2 = FPokemonTypeTable::GetMultiplier(PokemonDamage.Type, DefaultStatData.Type2);

		// 속성 데미지 계산 
		FinalDMG = PokemonDamage.Power * MulType1 * MulType2;

		// 특수 공격 및 공격 판정 후 특수 방어와 방어 중 선택
		if (PokemonDamage.IsSpecial)
		{
			DefenseStat = CalculateCurrentStat().SPD * 0.01f;
		}
		else
		{
			DefenseStat = CalculateCurrentStat().DEF * 0.01f;
		}

		// 방어력 공격력의 20% 정도 반영
		FinalDMG = FinalDMG - (FinalDMG * 0.2f * DefenseStat);
	}
	else
	{
		FinalDMG = DamageAmount;
	}

	// 데미지는 언제나 0 이상
	if (FinalDMG < ZERO) { FinalDMG = ZERO; }

	// 최종 데미지 계산
	CurrentHP = FMath::Max(CurrentHP - FinalDMG, ZERO);
	if (CurrentHP == ZERO) { PokemonDown(); }

	DamageCauserArray.Add(DamageCauser);

	return FinalDMG;
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
