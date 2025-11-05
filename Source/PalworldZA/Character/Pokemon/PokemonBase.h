// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/PokemonInterface/CommandReceiver.h"
#include "Interface/PokemonInterface/PokemonDataGetter.h"
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
	public IPokemonDataGetter
{
	GENERATED_BODY()

public: // 공개 함수

	APokemonBase();

	// 스킬 사용중인지 여부를 반환하는 함수
	FORCEINLINE uint8 GetIsOnSkill() { return ActionState == EPokemonAction::OnSkill; }

	// 스킬 종료시 호출
	void EndSkill();

public:	// 인터페이스 구현부 (ICommandReceiver) - 포켓몬이 받는

	// 스킬 사용
	virtual bool UsingSkill(int SkillNumber) override;

	// 포켓몬 활성화
	virtual bool SetActive(FVector Location) override;

	// 포켓몬 비활성화 
	virtual bool Deactive() override;

	// 포켓몬 쓰러졌을 때 호출할 델리게이트 등록
	virtual void BindOnPokemonDown(const FOnPokemonDown& InDelegate) override;

	// 새 타겟 설정
	virtual void SetTarget(AActor* NewTarget) override;

public:	// 인터페이스 구현부 (IPokemonDataGetter) - 포켓몬이 주는

	FORCEINLINE virtual AActor* GetTarget() { return CurrentSkillTarget; }

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

	// 스킬 타겟 업데이트
	void UpdateSkillTarget();

	// 경로를 통해 애니메이션 시퀀스 로드
	void LoadAnimSequenceData(FString Path);

	// 스킬 소환 함수
	ASkillBase* SpawnSkill(int SkillIndex);


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
	UPROPERTY()
	TArray<FSkillContainer> PokemonSkills;


protected: // 파라미터 변수

	FOnPokemonDown PokemonDownEvents;

	// 기본 포켓몬 스탯 데이터
	FPokemonStatData DefaultStatData;

	// 현재 포켓몬 스탯 데이터
	FPokemonStatData CurrentStatData;

	// 현재 포켓몬 행동 상태
	EPokemonAction ActionState = EPokemonAction::NonCommand;

	// Todo : PokemonBase 예상 추가 요소 
	// 노출 포켓몬 정보 Sturct (UI를 위함)
};
