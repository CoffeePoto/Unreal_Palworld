// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/PokemonInterface/CommandReceiver.h"
#include "Interface/PokemonInterface/PokemonDataGetter.h"
#include "Interface/PokemonInterface/HardCommandReceiver.h"
#include "Data/Pokemon/PokemonStatData.h"
#include "PokemonBase.generated.h"

class ASkillBase;

UENUM(BlueprintType)
enum class EPokemonAction : uint8
{
	NonCommand UMETA(DisplayName = "명령 수행 안함"),
	InCommand  UMETA(DisplayName = "명령 수행 중"),
	OnSkill    UMETA(DisplayName = "스킬 사용 중"),
	Down	   UMETA(DispalyName = "포켓몬 기절")
};


USTRUCT()
struct FSkillContainer
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TSubclassOf<ASkillBase> Skill;
	
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


public: // 인터페이스 구현부 (IHardCommandReceiver)

	// 스킬 실행 함수
	virtual void ExecuteSkill() override;


public:	// 인터페이스 구현부 (IPokemonDataGetter) - 포켓몬이 주는

	FORCEINLINE virtual AActor* GetTarget() override { return CurrentSkillTarget; }

	FORCEINLINE virtual uint8 IsOnSkill() override { return ActionState == EPokemonAction::OnSkill; }

	FORCEINLINE virtual FVector GetShootPoint() override { return GetActorLocation(); }


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

	// 스킬 타겟 업데이트
	void UpdateSkillTarget();

	// BB에 상태 업데이트
	void UpdateBBCommand();

	// 경로를 통해 애니메이션 시퀀스 로드
	void LoadAnimSequenceData(FString Path);

	// 스킬 소환 함수
	ASkillBase* SpawnSkill(int SkillIndex);

	// 원거리 공격 이동 위치 설정 함수
	void SetRangeAttackPosition();


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


protected: // 파라미터 변수

	// 포켓몬 기절 이벤트
	FOnPokemonDown PokemonDownEvents;

	// 포켓몬 기술 종료 이벤트
	FEndPokemonSkill PokemonSkillEndEvents;

	// 기본 포켓몬 스탯 데이터
	FPokemonStatData DefaultStatData;

	// 현재 포켓몬 스탯 데이터
	FPokemonStatData CurrentStatData;

	// 현재 포켓몬 행동 상태
	EPokemonAction ActionState = EPokemonAction::NonCommand;

	// 현재 선택된 스킬 번호
	uint8 SelectSkillNumber;

	// 현재 버프 쿨타임
	TArray<float> RemainingBuffTimes;
};
