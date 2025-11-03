// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CommandReceiver.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCommandReceiver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PALWORLDZA_API ICommandReceiver
{
	GENERATED_BODY()

	// 스킬 사용 코드 추후 매개변수 및 리턴 값은 조정후 추가하겠습니다.
	// 예측 매개변수는 (사용 스킬, 스킬 타겟, 스킬 종료후 호출할 함수) 입니다.
	// Todo : 명령 인터페이스 업데이트 

	virtual void UsingSkill() = 0;
	//virtual void UsingItem() = 0;
	
public:
};
