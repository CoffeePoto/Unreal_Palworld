
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LineTraceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PALWORLDZA_API ULineTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULineTraceComponent();

	void PerformLineTrace();
		
	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceDistance = 500.f;
};
