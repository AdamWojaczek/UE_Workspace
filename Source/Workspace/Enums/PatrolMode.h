#pragma once

#include "CoreMinimal.h"
#include "PatrolMode.generated.h"

UENUM(BlueprintType)
enum class EPatrolMode : uint8
{
	Loop UMETA(DisplayName = "Loop"),
	PingPong UMETA(DisplayName = "Ping Pong"),
	Random UMETA(DisplayName = "Random")
};