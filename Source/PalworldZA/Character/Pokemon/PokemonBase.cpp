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
#include "Data/Pokemon/PokemonSkillDataAsset.h"


APokemonBase::APokemonBase()
{
	//PrimaryActorTick.bCanEverTick = true;

	// 포켓몬 AI 컨트롤러를 APokemonAIController로 지정.
	AIControllerClass = APokemonAIController::StaticClass();
	
	// 모두 AIController에 빙의되도록 설정.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// ABP_PokemonBase를 따르도록 설정.
	static ConstructorHelpers::FClassFinder<UAnimInstance> PokemonAnimRef(TEXT("/Game/Animation/Pokemon/ABP_PokemonBase.ABP_PokemonBase_C"));
	if (PokemonAnimRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(PokemonAnimRef.Class);
	}

	CurrentHP = 1.0f;
}

// @Todo: 지금 당장은 안 쓰는 중
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
	BuffOrDebuffArray.SetNum((uint8)EPokemonBuffStat::COUNT);

	for (int i = 0; i < (uint8)EPokemonBuffStat::COUNT; ++i)
	{
		RemainingBuffTimes[i] = ZERO;
		BuffOrDebuffArray[i] = 0;
	}

	// 블랙 보드 연결
	BBComponent = Cast<APokemonAIController>(GetController())->GetBlackboardComponent();	
}

void APokemonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 스킬 쿨 타임 감소
	SkillCoolDown(DeltaTime);

	// 버프 시간 감소
	DownRemainingBuffTime(DeltaTime);

	// 데미지 주체 큐 관리
	DamageCauserQueueDequeue(DeltaTime);

	// 스킬 타겟 업데이트 
	UpdateSkillTarget();

	// 블랙 보드 업데이트 
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
	for (int i = 0; i < RemainingBuffTimes.Num(); ++i)
	{
		float& Time = RemainingBuffTimes[i];

		if (Time > ZERO)
		{
			Time = FMath::Max(Time - DeltaTime, ZERO);
		}
		else
		{
			BuffOrDebuffArray[i] = 0;
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
		CurrentDCDCycleTime = ZERO;

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
	UClass* SkillType = PokemonSkills[SkillIndex].Skill->Skill.Get();
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

	SpawnSk->SetSkillData(PokemonSkills[SkillIndex].Skill->Data);
	SpawnSk->SetUser(this);
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
	
	ReturnStat.Type1 = DefaultStatData.Type1;
	ReturnStat.Type2 = DefaultStatData.Type2;

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
	uint8 Index = static_cast<uint8>(Stat);

	if (BuffOrDebuffArray[Index] == 0) { return DefaultStat; }
	DefaultStat *= (BuffOrDebuffArray[Index] == 1 ? 1.5f : 0.5f);
	
	return DefaultStat;
}

void APokemonBase::PokemonDown()
{
	BBComponent->SetValueAsBool(BBKEY_ON_DOWN, true);

	FTimerHandle DeactiveTimer;

	// 타이머
	GetWorldTimerManager().SetTimer(
		DeactiveTimer,
		this,
		&APokemonBase::PokemonDownEventFunc,  
		3.0f,                     
		false                     
	);
}

void APokemonBase::PokemonDownEventFunc()
{
	Deactive();

	if (PokemonDownEvents.IsBound())
	{
		PokemonDownEvents.Execute();
	}
}

void APokemonBase::ExecuteSkill()
{
	if (ActionState == EPokemonAction::Down) { return; }

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

	/*if (PokemonSkillStartEvents.IsBound())
	{
		PokemonSkillStartEvents.Broadcast(PokemonSkills[SelectSkillNumber].Skill->Data.Name);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("실패"));
	}*/
}

float APokemonBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (ActionState == EPokemonAction::Down) { return ZERO; }
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

		// 방어력 공격력의 30% 정도 반영
		FinalDMG = FinalDMG - (FinalDMG * 0.3f * DefenseStat);
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

	UE_LOG(LogTemp, Log, TEXT("맞은 데미지: %f"), FinalDMG);
	DamageCauserArray.Add(DamageCauser);

	return FinalDMG;
}

bool APokemonBase::UsingSkill(uint8 SkillNumber)
{
	if (ActionState != EPokemonAction::NonCommand) { return false; }
	if (!PokemonSkills.IsValidIndex(SkillNumber)) { return false; }
	if (PokemonSkills[SkillNumber].CoolDown > ZERO) { return false; }

	ASkillBase* DefaultSkill = PokemonSkills[SkillNumber].Skill->Skill.Get()->GetDefaultObject<ASkillBase>();
	if (!DefaultSkill) { return false; }
	
	SelectSkillNumber = SkillNumber;
	ActionState = EPokemonAction::InCommand;

	FPokemonSkillData SkillData = PokemonSkills[SkillNumber].Skill->Data;
	
	BBComponent->SetValueAsEnum(BBKEY_ACTION_TYPE, static_cast<uint8>(SkillData.ActionType));
	if (SkillData.ActionType == EActionType::RANGE)
	{
		SetRangeAttackPosition();
	}

	if (PokemonSkillStartEvents.IsBound())
	{
		PokemonSkillStartEvents.Broadcast(PokemonSkills[SelectSkillNumber].Skill->Data.Name);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("실패"));
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
	if (ActionState != EPokemonAction::NonCommand &&
		ActionState != EPokemonAction::Down) 
	{
		return false;
	}

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

FDelegateHandle APokemonBase::BindStartPokemonSkill(const FStartPokemonSkill::FDelegate& InDelegate)
{
	return PokemonSkillStartEvents.Add(InDelegate);
}

void APokemonBase::UnBindStartPokemonSkill(FDelegateHandle Handle)
{
	PokemonSkillStartEvents.Remove(Handle);
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

	if (BuffOrDebuffArray[StatNumber] == -1 && RemainingBuffTimes[StatNumber] > ZERO)
	{
		BuffOrDebuffArray[StatNumber] = 0;
		RemainingBuffTimes[StatNumber] = ZERO;
		return;
	}

	if (IsCover)
	{
		BuffOrDebuffArray[StatNumber] = 1;
		RemainingBuffTimes[StatNumber] = Time;
	}
	else
	{
		BuffOrDebuffArray[StatNumber] = 1;
		RemainingBuffTimes[StatNumber] += Time;
	}

	if(RemainingBuffTimes[StatNumber] < 0) { RemainingBuffTimes[StatNumber] = 0; }
}

void APokemonBase::SetDeBuff(EPokemonBuffStat Stat, float Time, bool IsCover)
{
	uint8 StatNumber = static_cast<uint8>(Stat);
	if (!RemainingBuffTimes.IsValidIndex(StatNumber)) { return; }

	if (BuffOrDebuffArray[StatNumber] == 1 && RemainingBuffTimes[StatNumber] > ZERO)
	{
		BuffOrDebuffArray[StatNumber] = 0;
		RemainingBuffTimes[StatNumber] = ZERO;
		return;
	}

	if (IsCover)
	{
		BuffOrDebuffArray[StatNumber] = -1;
		RemainingBuffTimes[StatNumber] = Time;
	}
	else
	{
		BuffOrDebuffArray[StatNumber] = -1;
		RemainingBuffTimes[StatNumber] += Time;
	}

	if (RemainingBuffTimes[StatNumber] < 0) { RemainingBuffTimes[StatNumber] = 0; }
}

void APokemonBase::EndSkill()
{
	// 스킬 쿨타임 설정
	if (SpawnSkillController)
	{
		float CoolTime = SpawnSkillController->GetSkillData().Cooltime;
		CoolTime -= ((CoolTime * 0.3f) * (GetPokemonCurrentStat().Speed * 0.01f));
		PokemonSkills[SelectSkillNumber].CoolDown = CoolTime;
	}

	// 스폰된 스킬 제거
	if (SpawnedSkill)
	{
		SpawnedSkill->Destroy();
		SpawnSkillController = nullptr;
		SpawnedSkill = nullptr;
	}
	
	ActionState = EPokemonAction::NonCommand;
	if (PokemonSkillEndEvents.IsBound())
	{
		PokemonSkillEndEvents.Broadcast();
	}
}
