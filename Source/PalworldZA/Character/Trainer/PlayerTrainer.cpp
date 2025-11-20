// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Trainer/PlayerTrainer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Game/TrainerController.h"
#include "UI/PokemonHUD.h"
#include "UI/PokemonSlot.h"
#include "UI/PokemonStat.h"

#include "Character/Pokemon/PokemonBase.h"
#include "Interface/PokemonInterface/CommandReceiver.h"
//안넣고 싶었지만 타게팅을 위해서
#include "Interface/TrainerInterface/NPTrainerAIInterface.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


APlayerTrainer::APlayerTrainer()
	:IsFocusing(false)
{
	// 콜리전 프로파일 설정하는 함수.
	//GetCapsuleComponent()->SetCollisionProfileName(TEXT(""));

	//Camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	//Input Action c++ 상에서 미리 경로 설정 - 상속받은 블루프린트 생성때마다 지정하기 귀찮아서.
	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Game/Input/Trainer/IA_Look.IA_Look"));
	if (LookActionRef.Succeeded())
	{
		LookAction = LookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Game/Input/Trainer/IA_Move.IA_Move"));
	if (MoveActionRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SkillActionRef(TEXT("/Game/Input/Trainer/IA_Skill.IA_Skill"));
	if (SkillActionRef.Succeeded())
	{
		SkillAction = SkillActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> FocusActionRef(TEXT("/Game/Input/Trainer/IA_Focus.IA_Focus"));
	if (FocusActionRef.Succeeded())
	{
		FocusAction = FocusActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SelectActionRef(TEXT("/Game/Input/Trainer/IA_Select.IA_Select"));
	if (SelectActionRef.Succeeded())
	{
		SelectAction = SelectActionRef.Object;
	}
}

void APlayerTrainer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	//UI
	ATrainerController* MyController = Cast<ATrainerController>(GetController());
	if (MyController)
	{
		UPokemonHUD* UI = MyController->GetHUDWidget();
		PokemonUI = Cast<UPokemonHUD>(MyController->GetHUDWidget());
		ensureAlways(PokemonUI);
	}

	for (int i = 0; i < 6; ++i)
	{
		//CurrentPokemon = GetWorld()->SpawnActor<APokemonBase>(PokemonClass);

		FActorSpawnParameters SpawnParams;

		// 파라미터 값 세팅
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// 포켓몬 스폰 
		CurrentPokemon = GetWorld()->SpawnActor<APokemonBase>(
			PokemonClassArray[i],
			this->GetActorLocation(),
			GetActorRotation(),
			SpawnParams
		);

		if (CurrentPokemon)
		{
			CurrentPokemon->SetTrainer(this);
			CurrentPokemon->Deactive();

			Pokemons.Add(CurrentPokemon);
		}

		//UI 정보 반영
		PokemonUI->SetSlotThumbnail(i, CurrentPokemon->GetPokemonName());
	}

	CurrentPokemon = nullptr;
	SelectedPokemon = -1;
}

void APlayerTrainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//이젠 필요없음.

}

void APlayerTrainer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerTrainer::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerTrainer::Move);
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &APlayerTrainer::SkillMode);
		EnhancedInputComponent->BindAction(FocusAction, ETriggerEvent::Triggered, this, &APlayerTrainer::FocusOn);
		EnhancedInputComponent->BindAction(FocusAction, ETriggerEvent::Completed, this, &APlayerTrainer::FocusEnd);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &APlayerTrainer::SelectPokemonorSkill);
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &APlayerTrainer::Throw);
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* InputSystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (InputSystem)
		{
			InputSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayerTrainer::FocusOn()
{
	if (CurrentPokemon == nullptr) return;

	const float DetectRange = 2000.0f;

	FVector HalfSize(
		DetectRange * 0.5f,  // 앞뒤 길이
		300.f,               // 좌우 범위 (좁게)
		600.f                // 위아래 범위 (좁게)
	);

	// 시작 위치
	FVector Start = Camera->GetComponentLocation() + Camera->GetForwardVector() * 30.f;

	// 종료 위치
	FVector End = Start + Camera->GetForwardVector() * DetectRange;

	// 카메라 방향 (쿼터니엄)
	FQuat Rot = Camera->GetComponentRotation().Quaternion();

	TArray<FHitResult> HitResults;

	FCollisionQueryParams Params(
		SCENE_QUERY_STAT(TrainerDetect),
		false,
		this
	);

	bool HitDetected = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		Rot,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeBox(HalfSize),
		Params
	);

	if (!HitDetected) { return; }

	for (FHitResult& HitResult : HitResults)
	{
		APokemonBase* TargetPokemon = Cast<APokemonBase>(HitResult.GetActor());

		if (!TargetPokemon) { continue; }

		const APawn* Trainer = TargetPokemon->GetTrainer();
		if (Trainer && Trainer->GetController() == UGameplayStatics::GetPlayerController(this, 0))
		{
			continue;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("NPC포켓몬 탐색 성공"));
			CurrentPokemon->SetTarget(TargetPokemon);
			break;
		}
	}

	// 디버그 모드일 때만 그리도록.
#if ENABLE_DRAW_DEBUG

	// 캡슐의 중심 위치.
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;

	// 캡슐 높이의 절반 값.
	float CapsuleHalfHeight = DetectRange * 0.5f;

	// 색상 (그리기 색상).
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	// 충돌 디버그 (시각적 도구 활용).
	DrawDebugBox(
		GetWorld(),
		Start + Camera->GetForwardVector() * (DetectRange * 0.5f), // 중앙
		HalfSize,
		Rot,
		FColor::Green,
		false,
		3.0f
	);
#endif
	return;

}




// if (CurrentPokemon == nullptr) return;
//
//	//키입력 F가 입력되고 있으면 FocusOn 함수 호출
//	//Test
//	UE_LOG(LogTemp, Log, TEXT("FocusOn 함수 호출"));
//
//	const float DetectRange = 800.0f;
//	const float DetectRadius = 150.0f;
//
//	FHitResult HitTarget;
//	FVector Start = GetActorLocation()
//		+ GetActorForwardVector() 
//		* GetCapsuleComponent()->GetScaledCapsuleRadius();
//
//	FVector End = Start + GetActorForwardVector() * DetectRange;//감지거리 hardcoding
//
//	//FVector Start = Camera->GetComponentLocation() + FVector(0.0f, 0.0f, 40.0f);
//	//FVector End = Start + Camera->GetForwardVector() * DetectRange;
//
//	FCollisionQueryParams Params(
//		SCENE_QUERY_STAT(TrainerDetect),
//		false,
//		this
//	);
//
//	bool HitDetected = GetWorld()->SweepSingleByChannel
//	(
//		HitTarget,
//		Start,
//		End,
//		FQuat::Identity,
//		ECollisionChannel::ECC_GameTraceChannel1,// == pokemon trace channel
//		FCollisionShape::MakeSphere(DetectRadius),
//		Params
//	);
//
//	//충돌 발생
//	if (HitDetected)
//	{
//		UE_LOG(LogTemp, Log, TEXT("충돌 확인"));
//		//충돌 결과를 포켓몬으로 캐스팅
//		APokemonBase* TargetPokemon = Cast<APokemonBase>(HitTarget.GetActor());
//		if (TargetPokemon)
//		{
//			const APawn* Trainer = TargetPokemon->GetTrainer();
//			{
//				if (Trainer)
//				{
//					if (Trainer->GetController() == UGameplayStatics::GetPlayerController(this, 0)) return;
//					else
//					{
//						UE_LOG(LogTemp, Log, TEXT("NPC포켓몬 탐색 성공"));
//						CurrentPokemon->SetTarget(TargetPokemon);
//					}
//				}
//				else
//				{
//					UE_LOG(LogTemp, Log, TEXT("포켓몬 탐색 성공"));
//					Pokemons[SelectedPokemon]->SetTarget(TargetPokemon);
//				}
//			}
//		}
//		else
//		{
//			UE_LOG(LogTemp, Log, TEXT("트레이너 충돌"));
//			//충돌 결과를 Interface로 캐스팅
//			INPTrainerAIInterface* OpponentPlayer = Cast<INPTrainerAIInterface>(HitTarget.GetActor());
//			if (OpponentPlayer)
//			{
//				if (OpponentPlayer->GetPokemon() == nullptr) return;
//				APokemonBase* NPCPokemon = Cast<APokemonBase>(OpponentPlayer->GetPokemon());
//				if (!NPCPokemon) return;
//				UE_LOG(LogTemp, Log, TEXT("NPC 트레이너 소유 포켓몬 주시"));
//				CurrentPokemon->SetTarget(NPCPokemon);
//			}
//		}
//	}
//
//	// 디버그 모드일 때만 그리도록.
//#if ENABLE_DRAW_DEBUG
//
//	// 캡슐의 중심 위치.
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//
//	// 캡슐 높이의 절반 값.
//	float CapsuleHalfHeight = DetectRange * 0.5f;
//
//	// 색상 (그리기 색상).
//	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
//
//	// 충돌 디버그 (시각적 도구 활용).
//	DrawDebugCapsule(
//		GetWorld(),
//		CapsuleOrigin,
//		CapsuleHalfHeight,
//		DetectRadius,
//		//FRotationMatrix::MakeFromZ(Camera->GetForwardVector()).ToQuat(),
//		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
//		DrawColor,
//		false,
//		5.0f
//	); 
//#endif

void APlayerTrainer::FocusEnd()
{
	IsFocusing = false;
}

void APlayerTrainer::SelectPokemonorSkill(const FInputActionValue& value)
{
	float SelectedIndex = value.Get<float>() - 1;
	uint8 intIndex = (uint8)SelectedIndex;
	if (UseSkill)
	{
		if (intIndex < 4)
		{
			//for test
			UE_LOG(LogTemp, Log, TEXT("Skill Mode : %d"), intIndex);
			//Pokemons[SelectedPokemon]->UsingSkill(intIndex);
			CommandSkills(intIndex);
			// 걷다가 멈추고 skill
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

			// Todo. 몽타주
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(SkillActionMontage, 1.5f);

			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &APlayerTrainer::ReleaseSkillMode);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, SkillActionMontage);
		}
	}
	else
	{
		//현재 트레이너가 가지고 있는 포켓몬 개수만큼만 이동 가능
		uint8 Num = static_cast<uint8>(Pokemons.Num());
		uint8 ClampIndex = (intIndex < Num) ? intIndex : (Num > 0 ? Num - 1 : 0);

		//전과 같은 번호를 입력했다면, 입력 무시
		if (SelectedPokemon == ClampIndex) return;
		SetSelectedPokemon(ClampIndex);
		SelectedPokemon = ClampIndex;

		//UI에 변경사항 반영
		PokemonUI->SelectUI(ClampIndex);

		//for test 
		UE_LOG(LogTemp, Log, TEXT("Current Index : %d"), SelectedPokemon);
	}
}

void APlayerTrainer::Move(const FInputActionValue& value)
{
	// 던지기 스킬 진행 중일 때는 이동 처리 안함.
	if (IsThrowing)
	{
		return;
	}

	// 입력 값 읽어오기.
	FVector2D Movement = value.Get<FVector2D>();

	// 이동할 방향 만들기.
	// 카메라가 바라보는 방향(컨트롤러의 방향)을 기준으로 이동 방향 만들기.
	FRotator Rotation = GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	// 앞방향.
	FVector ForwardVector
		= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// 오른쪽 방향.
	FVector RightVector
		= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 무브먼트에 입력 전달하기.
	// 크기가 1인 숫자를 곱하는 행위는 "회전"
	AddMovementInput(ForwardVector, Movement.Y);
	AddMovementInput(RightVector, Movement.X);
}

void APlayerTrainer::Look(const FInputActionValue& value)
{
	// 입력 값 읽어오기.
	FVector2D LookValue = value.Get<FVector2D>();
	//카메라 속도 조절을 위한 float 값 배정
	double CameraSpeed = 0.7f;

	// 컨트롤러에 회전 적용.
	// 마우스 좌우 드래그 입력을 컨트롤러의 Z축 회전(요, Yaw)에 적용.
	AddControllerYawInput(LookValue.X * CameraSpeed);

	// 마우스 좌우 드래그 입력을 컨트롤러의 Y축 회전(피치, Pitch)에 적용.
	AddControllerPitchInput(LookValue.Y * CameraSpeed);
}

void APlayerTrainer::SkillMode()
{

	// 이미 스킬 진행 중이면 종료.
	if (UseSkill)
	{
		return;
	}

	// 스킬 시작.
	UseSkill = true;



	// 1) 지금 선택된 내 포켓몬 가져오기
	APokemonBase* MyPokemon = nullptr;
	if (Pokemons.IsValidIndex(SelectedPokemon))
	{
		MyPokemon = Pokemons[SelectedPokemon];
	}

	// 2) 그 포켓몬이 바라보고 있는 타겟 포켓몬 가져오기
	if (MyPokemon)
	{
		APokemonBase* TargetPokemon = Cast<APokemonBase>(MyPokemon->GetTarget()); 

		if (TargetPokemon && IsValid(TargetPokemon))
		{
			FVector MyLoc = GetActorLocation();
			FVector TargetLoc = TargetPokemon->GetActorLocation();

			// 수평 방향만 사용 (위/아래 각도는 무시)
			FVector Dir = TargetLoc - MyLoc;
			Dir.Z = 0.0f;

			if (!Dir.IsNearlyZero())
			{
				FRotator LookRot = Dir.Rotation();
				LookRot.Pitch = 0.0f;
				LookRot.Roll = 0.0f;

				SetActorRotation(LookRot);
			}
		}
	}


	//// 입력 통째로 막기
	//if (APlayerController* PC = Cast<APlayerController>(GetController()))
	//{
	//	DisableInput(PC);
	//}

	// 걷다가 멈추고 skill
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;

	//몽타주 애니메이션
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(SkillActionMontage, 1.5f);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APlayerTrainer::ReleaseSkillMode);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, SkillActionMontage);
}

void APlayerTrainer::ReleaseSkillMode(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// skill 종료.
	UseSkill = false;

	//// 입력 다시 켜기
	//if (APlayerController* PC = Cast<APlayerController>(GetController()))
	//{
	//	EnableInput(PC);
	//}

	 // 걷기 모드로 복귀.
    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->SetMovementMode(EMovementMode::MOVE_Walking);
		MoveComp->MaxWalkSpeed = 700.0f;
    }
}

void APlayerTrainer::Throw()
{
	//아무것도 없으면 return
	if (Pokemons.IsEmpty() || SelectedPokemon < 0) return;
	//포켓몬 위치 잡고 소환
	//이미 소환된 포켓몬과 소환할 포켓몬이 같은 경우 무시
	if (CurrentPokemon == Pokemons[SelectedPokemon]) return;
	//이미 소환된 포켓몬을 deactivate
	if (CurrentPokemon)
	{
		if (!CurrentPokemon->Deactive())
		{
			return;
		}
	}

	//새로운 포켓몬을 소환
	SummonPokemon();

	//UI에 포켓몬 정보 반영
	UPokemonStat* PokemonInfo = PokemonUI->GetStatUI();
	PokemonInfo->SetDeadInfoOff();
	PokemonInfo->SetPokemonNameandThumbnail(CurrentPokemon->GetPokemonName());
	PokemonInfo->SetMaxHp((int)CurrentPokemon->GetPokemonCurrentStat().Hp);
	PokemonInfo->SetLevel(100);
	PokemonInfo->UpdateCurrentHp(CurrentPokemon->GetPokemonHp());
	PokemonInfo->SetTypeImage(CurrentPokemon->GetPokemonDefaultStat().Type1, CurrentPokemon->GetPokemonDefaultStat().Type2);

	//델리게이트 연결
	ICommandReceiver* Commander = Cast<ICommandReceiver>(CurrentPokemon);
	FHitPokemon::FDelegate HitEventDelegate;
	HitEventDelegate.BindUObject(this,&APlayerTrainer::HitDelegateEntrance);
	FDelegateHandle Handle = Commander->BindHitPokemon(HitEventDelegate);
	FOnPokemonDown DownEventDelegate;
	DownEventDelegate.BindLambda([PokemonInfo]() { PokemonInfo->ShowDeadEvent(); });
	Commander->BindOnPokemonDown(DownEventDelegate);

	// 애니메이션 part
	// 이미 던지기 진행 중이면 종료.
	if (IsThrowing)
	{
		return;
	}

	// 던지기 시작.
	IsThrowing = true;

	// 걷다가 멈추고 throw
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Todo. 몽타주
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ThrowActionMontage, 1.0f);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APlayerTrainer::ThrowActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ThrowActionMontage);
}

void APlayerTrainer::ThrowActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// 던지기 종료.
	IsThrowing = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void APlayerTrainer::HitDelegateEntrance()
{
	UPokemonStat* PokemonInfo = PokemonUI->GetStatUI();
	//변화된 포켓몬의 현재 체력을 읽어온다.
	//공격받았다는 델리게이트가 호출된 후 실행되므로,
	//UI의 CurrentHp와 포켓몬의 CurrentHp는 다를 것이다.
	PokemonInfo->OnHpChanged(CurrentPokemon->GetPokemonHp());
}
