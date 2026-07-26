#pragma once

#include "CoreMinimal.h"
#include "EffectStackingPolicy.generated.h"



UENUM(BlueprintType)
enum class EEffectStackingPolicy : uint8
{
	IgnoreIfActive,
	RefreshDuration,
	Stack
};
