#include "Components/LineTraceComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"

ULineTraceComponent::ULineTraceComponent()
{
    // Tick 비활성화 (매 프레임 수행할 필요가 없음)
    PrimaryComponentTick.bCanEverTick = false;
}

void ULineTraceComponent::PerformLineTrace()
{
    // 오너 확인
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("[LineTraceComponent] No Owner"));
        return;
    }

    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC) return;

    FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
    FRotator CameraRotation = PC->PlayerCameraManager->GetCameraRotation();

    FVector TraceStart = CameraLocation;
    FVector TraceEnd = TraceStart + (CameraRotation.Vector() * TraceDistance);

    FHitResult HitResult;
    FCollisionQueryParams Params;  //예외처리
    Params.AddIgnoredActor(PC->GetPawn());

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        TraceStart,
        TraceEnd,
        ECC_Visibility,
        Params
    );

#if WITH_EDITOR
    DebugDrawLine(TraceStart, TraceEnd);
#endif #if WITH_EDITOR 

    if (bHit && HitResult.GetActor())
    {
        UE_LOG(LogTemp, Warning, TEXT("LineTraceComponent] Hit Actor: %s"), *HitResult.GetActor()->GetName());
        return;
    }
}

void ULineTraceComponent::DebugDrawLine(const FVector& Start, const FVector& End)
{
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f, 0, 0.1f);
}

