// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Pokemon/PokemonBillboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/PokemonBillboard/PokemonBillboard.h"
#include "UI/PokemonBillboard/PokemonBuffBillboard.h"
#include "UI/PokemonBillboard/PokemonDataBillboard.h"
#include "Interface/PokemonInterface/CommandReceiver.h"
#include "Interface/PokemonInterface/PokemonDataGetter.h"

UPokemonBillboardComponent::UPokemonBillboardComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	// 위젯 설치 공간 설정
	SetWidgetSpace(EWidgetSpace::World);

	// 위젯을 DrawSize로 그리도록 설정
	SetDrawAtDesiredSize(false);

	// 위젯 원본 사이즈로 지정
	SetDrawSize(FVector2D(1280.f, 720.f));

	// 피벗을 화면 중앙 하단으로 설정 
	SetPivot(FVector2D(0.5f, 0.0f));

	// 전체 사이즈 스케일링 
	SetWorldScale3D(FVector(0.08f));   

	// 위젯 위치 설정 
	SetRelativeLocation(FVector(0.0f, 0.0f, 150.f));

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(
		TEXT("/Game/UI/Pokemon/WBP_PokemonBillboard.WBP_PokemonBillboard_C")
	);

	if (WidgetClassFinder.Succeeded())
	{
		SetWidgetClass(WidgetClassFinder.Class);
	}
}

void UPokemonBillboardComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) return;

	if (APokemonBase* PokemonOwner = Cast<APokemonBase>(OwnerActor))
	{
		Pokemon = PokemonOwner;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("이 컴포넌트는 포켓몬 베이스 전용입니다."));
	}

	Billboard = Cast<UPokemonBillboard>(GetUserWidgetObject());

	if (!Billboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("잘못된 위젯을 불러오고 있습니다."));
	}

	Billboard->SetPokemon(Pokemon);
	UpdateName();	
}

void UPokemonBillboardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	LocationBillboard();
	//CheackOwner();

	if (IsNeedViewer)
	{
		UpdateBuff();
		UpdateHp();
		UpdateType();
	}
}

void UPokemonBillboardComponent::LocationBillboard()
{
	if (GetWidgetSpace() == EWidgetSpace::World)
	{
		if (APlayerCameraManager* Cam = UGameplayStatics::GetPlayerCameraManager(this, 0))
		{
			const FVector ToCam = Cam->GetCameraLocation() - GetComponentLocation();

			// 회전값 계산
			const FRotator LookAtRot = FRotationMatrix::MakeFromX(ToCam).Rotator();

			// 빌보드 회전 적용
			SetWorldRotation(LookAtRot);
		}
	}
}

void UPokemonBillboardComponent::UpdateName()
{
	IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(Pokemon);
	if (!Getter) { return; }

	Billboard->GetDataBB()->UpdateName(Getter->GetPokemonName());
}

void UPokemonBillboardComponent::UpdateLv()
{
	IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(Pokemon);
	if (!Getter) { return; }
}

void UPokemonBillboardComponent::UpdateType()
{
	IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(Pokemon);
	if (!Getter) { return; }

	Billboard->GetDataBB()->UpdateType(
		Getter->GetPokemonDefaultStat().Type1,
		Getter->GetPokemonDefaultStat().Type2
	);
}

void UPokemonBillboardComponent::UpdateBuff()
{
	IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(Pokemon);
	if (!Getter) { return; }


	Billboard->GetBuffBB()->UpdateStatBuff(Getter->GetBuffState());
}

void UPokemonBillboardComponent::UpdateHp()
{
	IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(Pokemon);
	if (!Getter) { return; }

	Billboard->SetHpBar(Getter->GetPokemonHp() / Getter->GetPokemonDefaultStat().Hp);
}

void UPokemonBillboardComponent::CheackOwner()
{
	if (Pokemon)
	{
		IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(Pokemon);
		if (!Getter) { return; }

		const APawn* Trainer = Getter->GetTrainer();
		if (!Trainer) { return; }

		if (Trainer->GetController() == UGameplayStatics::GetPlayerController(this, 0))
		{
			IsNeedViewer = false;
			Billboard->SetPlayerPokemonView();
		}
		else
		{
			IsNeedViewer = true;
			Billboard->SetNonPlayerPokemonView();
		}
	}
}
