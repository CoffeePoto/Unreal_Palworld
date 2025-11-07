
#include "Components/LineTraceComponent.h"
#include "Camera/CameraComponent.h"


ULineTraceComponent::ULineTraceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULineTraceComponent::PerformLineTrace()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    // 캐릭터의 카메라를 찾기
    UCameraComponent* Camera = Owner->FindComponentByClass<UCameraComponent>();
    if (!Camera)
    {
        UE_LOG(LogTemp, Warning, TEXT("[LineTraceComponent] No Camera Found"));
        return;
    }

    FVector Start = Camera->GetComponentLocation();
    FVector End = Start + (Camera->GetForwardVector() * TraceDistance);

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(LineTrace), false, Owner);

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red, false, 1.0f, 0, 1.5f);

    if (bHit && Hit.GetActor())
    {
        UE_LOG(LogTemp, Warning, TEXT("[LineTraceComponent] Hit Actor: %s"),
            *Hit.GetActor()->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[LineTraceComponent] No Hit"));
    }
}

