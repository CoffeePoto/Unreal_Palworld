// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Trainer/TrainerBase.h"
#include "InputActionValue.h"
#include "PlayerTrainer.generated.h"

/**
 * 
 */
class ULineTraceComponent;
UCLASS()
class PALWORLDZA_API APlayerTrainer : public ATrainerBase
{
	GENERATED_BODY()
	
public:
	APlayerTrainer();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//플레이어가 상대 포켓몬을 주시한다.
	void FocusOn();
	void FocusEnd();

	//플레이어가 포켓몬을 선택한다.
	void SelectPokemon(const FInputActionValue& value);

	//이동 로직
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void Run();
	void RunEnd();

	// 공격 로직
	void Throw(const FInputActionValue& value);
	void ThrowActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

protected:
	//Camera Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	//Input Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RunAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FocusAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SelectAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ThrowAction;

	//focus 여부 판단 
	bool isFocusing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ThrowActionMontage;

	// 던지기 스킬 중인지 판단.
	bool IsThrowing;

	// 라인트레이스 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trace", meta = (AllowPrivateAccess = "true"))
	ULineTraceComponent* LineTraceComponent = nullptr;
};
