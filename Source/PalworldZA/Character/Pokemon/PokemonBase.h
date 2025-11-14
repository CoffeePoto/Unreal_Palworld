// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/PokemonInterface/CommandReceiver.h"
#include "Interface/PokemonInterface/PokemonDataGetter.h"
#include "Interface/PokemonInterface/HardCommandReceiver.h"
#include "Data/Pokemon/PokemonStatData.h"
#include "PokemonBase.generated.h"

#define ZERO 0.0f

class UPokemonSkillDataAsset;
class ASkillBase;

UENUM(BlueprintType)
enum class EPokemonAction : uint8
{
	NonCommand UMETA(DisplayName = "명령 수행 안함"),
	InCommand  UMETA(DisplayName = "명령 수행 중"),
	OnSkill    UMETA(DisplayName = "스킬 사용 중"),
	Down	   UMETA(DisplayName = "포켓몬 기절")
};


USTRUCT()
struct FSkillContainer
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<UPokemonSkillDataAsset> Skill;
	
	UPROPERTY()
	float CoolDown;
};


UCLASS()
class PALWORLDZA_API APokemonBase :
	public ACharacter,
	public ICommandReceiver,
	public IHardCommandReceiver,
	public IPokemonDataGetter

{
	GENERATED_BODY()


public: // 공개 함수

	APokemonBase();

	// 스킬 종료시 호출
	void EndSkill();


public:	// 인터페이스 구현부 (ICommandReceiver) - 포켓몬이 받는

	// 스킬 사용
	virtual bool UsingSkill(uint8 SkillNumber) override;

	// 포켓몬 활성화
	virtual bool SetActive(FVector Location) override;

	// 포켓몬 비활성화 
	virtual bool Deactive() override;

	// 포켓몬 쓰러졌을 때 호출할 델리게이트 등록
	virtual void BindOnPokemonDown(const FOnPokemonDown& InDelegate) override;

	// 포켓몬 공격시 호출할 델리게이트 등록
	virtual FDelegateHandle BindStartPokemonSkill(const FStartPokemonSkill::FDelegate& InDelegate) override;

	// 포켓몬 공격시 호출할 델리게이트 해지
	virtual void UnBindStartPokemonSkill(FDelegateHandle Handle) override;

	// 포켓몬 기술 종료시 호출할 델리게이트 등록
	virtual FDelegateHandle BindEndPokemonSkill(const FEndPokemonSkill::FDelegate& InDelegate) override;

	// 포켓몬 기술 종료 호출 델리게이트 해지
	virtual void UnBindEndPokemonSkill(FDelegateHandle Handle) override;

	// 새 타겟 설정
	virtual void SetTarget(AActor* NewTarget) override;

	// 트레이너 설정
	virtual void SetTrainer(APawn* NewTrainer) override;

	// 버프 설정 / 매개 변수 : (버프 스탯, 시간, 버프 덮어쓰기 여부)
	virtual void SetBuff(EPokemonBuffStat Stat, float Time, bool IsCover = true) override;

	// 디버프 설정 / 매개 변수 : (디버프 스탯, 시간, 디버프 덮어쓰기 여부)
	virtual void SetDeBuff(EPokemonBuffStat Stat, float Time, bool IsCover = true) override;

public: // 인터페이스 구현부 (IHardCommandReceiver)

	// 스킬 실행 함수
	virtual void ExecuteSkill() override;

	virtual void ReservationSkill(int SkillNumber) override;

public:	// 인터페이스 구현부 (IPokemonDataGetter) - 포켓몬이 주는

	// 현재 포켓몬이 바라보는 타겟
	FORCEINLINE virtual AActor* GetTarget() override { return CurrentSkillTarget; }

	// 현재 포켓몬이 공격하는지 판단
	FORCEINLINE virtual uint8 IsOnSkill() override { return ActionState == EPokemonAction::OnSkill; }

	// 포켓몬 발사체 발사 위치
	FORCEINLINE virtual FVector GetShootPoint() override { return GetActorLocation(); }

	// 포켓몬 기본 스텟 정보 
	FORCEINLINE virtual const FPokemonStatData& GetPokemonDefaultStat() override { return DefaultStatData; }

	// 포켓몬 현재 스텟 정보
	FORCEINLINE virtual const FPokemonStatData GetPokemonCurrentStat() override { return CalculateCurrentStat(); }

	// 포켓몬 현재 체력 반환
	FORCEINLINE virtual const float GetPokemonHp() override { return CurrentHP; }

	// 현재 포켓몬의 트레이너 반환
	FORCEINLINE virtual const APawn* GetTrainer() override { return Trainer; }

	// 현재 포켓몬의 버프 상태 반환
	FORCEINLINE virtual const TArray<int8>& GetBuffState() const { return BuffOrDebuffArray; }

	// 포켓몬 이름 반환
	FORCEINLINE virtual const FString GetPokemonName() { return MyName; }

	// 포켓몬 쓰러졌는지 여부 반환
	FORCEINLINE virtual const bool GetIsPokemonDown() { return CurrentHP == ZERO; };

	virtual void GetSkillData(TArray<FCurrentPokemonSkillData>& OutArray) const override;

protected: // 오버라이딩 구현부 

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// 대미지 처리 함수 오버라이드.
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;


protected: // 자체 함수 구현부

	// 스킬 쿨타임 감소 함수
	void SkillCoolDown(float DeltaTime);

	// 버프 남은 시간 감소 함수
	void DownRemainingBuffTime(float DeltaTime);

	// 데미지 커서 큐 꺼내는 함수 
	void DamageCauserQueueDequeue(float DeltaTime);

	// 스킬 타겟 업데이트
	void UpdateSkillTarget();

	// BB에 상태 업데이트
	void UpdateBBCommand();

	// 타겟 활성화 체크 
	void TargetActiveCheck();

	// 경로를 통해 애니메이션 시퀀스 로드
	void LoadAnimSequenceData(FString Path);

	// 스킬 소환 함수
	ASkillBase* SpawnSkill(int SkillIndex);

	// 원거리 공격 이동 위치 설정 함수
	void SetRangeAttackPosition();

	// 현재 스탯 데이터 계산
	const FPokemonStatData CalculateCurrentStat();

	// 각각 파라미터 계산
	float CalculateStatParameters(EPokemonBuffStat Stat, float DefaultStat);

	// 포켓몬 기절 
	void PokemonDown();

	// 포켓몬 기절 이벤트 처리 함수
	void PokemonDownEventFunc();

	// 포켓몬이 피격받았을 때 데미지 처리 외 실행할 기능 
	void HitInnerEvent(APawn* Attacker);



protected: // Has 변수 

	// 자신을 소유한 트레이너 
	UPROPERTY()
	TObjectPtr<class APawn> Trainer;

	// BT를 제어하기 위한 BB컴포넌트
	UPROPERTY()
	TObjectPtr<class UBlackboardComponent> BBComponent;

	// 애니메이션 시퀀스 정보를 가진 데이터 에셋
	UPROPERTY()
	TObjectPtr<class UPokemonAnimSequenceData> AnimData;

	// 스킬을 사용할 타겟 
	UPROPERTY()
	TObjectPtr<class AActor> CurrentSkillTarget;

	// 새 스킬을 사용할 타겟
	UPROPERTY()
	TObjectPtr<class AActor> NewSkillTarget;
	
	// 스킬 관리 배열 
	UPROPERTY(Transient)
	TArray<FSkillContainer> PokemonSkills;

	// 소환된 스킬
	UPROPERTY()
	TObjectPtr<class ASkillBase> SpawnedSkill;

	// 소환된 스킬 컨트롤러
	UPROPERTY()
	TScriptInterface<class IPokemonSkill> SpawnSkillController;

	// 데미지를 준 주체를 저장하는 배열
	UPROPERTY()
	TArray<TObjectPtr<AActor>> DamageCauserArray;

protected: // 파라미터 변수

	// 포켓몬 기절 이벤트
	FOnPokemonDown PokemonDownEvents;

	// 포켓몬 기술 종료 이벤트
	FEndPokemonSkill PokemonSkillEndEvents;

	// 포켓몬 기술 사용 이벤트
	FStartPokemonSkill PokemonSkillStartEvents;

	// 기본 포켓몬 스탯 데이터
	FPokemonStatData DefaultStatData;

	// 현재 포켓몬 행동 상태
	EPokemonAction ActionState = EPokemonAction::NonCommand;

	// 현재 선택된 스킬 번호
	uint8 SelectSkillNumber;

	// 현재 버프 쿨타임
	TArray<float> RemainingBuffTimes;

	// 현재 버프 디버프 판별
	TArray<int8> BuffOrDebuffArray;

	// 현재 남은 체력
	float CurrentHP;

	// 지울 데미지 커서 인덱스
	int32 DamageCauserIndex;

	// 현재 더미지 커서 지우는 주기 시간
	float CurrentDCDCycleTime;

	// 현재 더미지 커서 지우는 시작 시간
	float CurrentDCDStartTime;

	// 포켓몬 이름 (임시)
	FString MyName;

	// 예약된 스킬 번호
	int ReservationSkillNumber = -1;

private: // Base내에서만 사용하는 상수
	
	// 데미지 커서 제거 시작 시간
	static constexpr float DAMAGE_CAUSER_DEQUEUE_START_TIME = 1.0f;

	// 데미지 커서 제거 주기 시간
	static constexpr float DAMAGE_CAUSER_DEQUEUE_CYCLE_TIME = 0.5f;
};
