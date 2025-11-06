// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Trainer/PlayerTrainer.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

APlayerTrainer::APlayerTrainer()
	:isFocusing(false)
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

	static ConstructorHelpers::FObjectFinder<UInputAction> RunActionRef(TEXT("/Game/Input/Trainer/IA_Run.IA_Run"));
	if (RunActionRef.Succeeded())
	{
		RunAction = RunActionRef.Object;
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
}

void APlayerTrainer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerTrainer::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerTrainer::Move);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &APlayerTrainer::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerTrainer::RunEnd);
		EnhancedInputComponent->BindAction(FocusAction, ETriggerEvent::Triggered, this, &APlayerTrainer::FocusOn);
		EnhancedInputComponent->BindAction(FocusAction, ETriggerEvent::Completed, this, &APlayerTrainer::FocusEnd);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Started, this, &APlayerTrainer::SelectPokemon);
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

	//주시할 포켓몬을 찾았을 경우에만 isFocusing 변수를 true로 만들 것.
	//주시할 포켓몬
	//APokemonBase* FocusedPokemon;
	//line trace 시작 지점 - 캐릭터가 소유한 카메라
	

	//if (GetWorld()->LineTraceSingleByChannel())
	//{

	//}
}

void APlayerTrainer::FocusEnd()
{
	isFocusing = false;
}

void APlayerTrainer::SelectPokemon(const FInputActionValue& value)
{
	float SelectedIndex = value.Get<float>() - 1;
	SetSelectedPokemon((uint8)SelectedIndex);
	//SelectedPokemon = (uint8)SelectedIndex;
	//for test 
	UE_LOG(LogTemp, Log, TEXT("Current Index : %d"), SelectedPokemon);
}

void APlayerTrainer::Move(const FInputActionValue& value)
{
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

void APlayerTrainer::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
}

void APlayerTrainer::RunEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}
