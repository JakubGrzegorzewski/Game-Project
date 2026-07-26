#pragma once

#include "CoreMinimal.h"
#include "AbilityTargetType.generated.h"


UENUM(BlueprintType)
enum class EAbilityTargetType : uint8
{
	Self,
	ActorTarget,
	AreaAtLocation,
	Projectile
};
