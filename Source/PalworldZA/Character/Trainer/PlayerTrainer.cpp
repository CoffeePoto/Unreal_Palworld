// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Trainer/PlayerTrainer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Game/TrainerController.h"
#include "UI/PokemonHUD.h"

#include "Character/Pokemon/AttackTestPokemon.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Components/LineTraceComponent.h"


APlayerTrainer::APlayerTrainer()
	:IsFocusing(false)
{
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

	// LineTraceComponent 생성
	LineTraceComponent = CreateDefaultSubobject<ULineTraceComponent>(TEXT("LineTraceComponent"));
}

void APlayerTrainer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}
}

void APlayerTrainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AAttackTestPokemon* FindPokemon = Cast<AAttackTestPokemon>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AAttackTestPokemon::StaticClass())
	);
	if (FindPokemon)
	{
		//UE_LOG(LogTemp, Log, TEXT("Find Pokemon"));
		FindPokemon->SetTrainer(this);
		Pokemons.Add(FindPokemon);
	}
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
	//키입력 F가 입력되고 있으면 FocusOn 함수 호출
	//Test
	UE_LOG(LogTemp, Log, TEXT("FocusOn 함수 호출"));
}

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
		}
	}
	else
	{
		//전과 같은 번호를 입력했다면, 입력 무시
		if (SelectedPokemon == intIndex) return;
		SetSelectedPokemon(intIndex);
		SelectedPokemon = (uint8)SelectedIndex;

		//UI에 변경사항 반영
		ATrainerController* MyController = Cast<ATrainerController>(GetController());
		if (MyController)
		{
			UPokemonHUD* UI = MyController->GetHUDWidget();
			UI->SelectUI(intIndex);
		}

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

void APlayerTrainer::SkillMode(const FInputActionValue& value)
{
	// UseSkill = true;

	// 이미 스킬 진행 중이면 종료.
	if (UseSkill)
	{
		return;
	}

	// 스킬 시작.
	UseSkill = true;

	// 걷다가 멈추고 skill
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Todo. 몽타주
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(SkillActionMontage, 1.5f);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APlayerTrainer::ReleaseSkillMode);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, SkillActionMontage);
}

void APlayerTrainer::ReleaseSkillMode(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	//UseSkill = false;

	// skill 종료.
	UseSkill = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void APlayerTrainer::Throw(const FInputActionValue& value)
{
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