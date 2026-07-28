#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilityCost.generated.h"

class UStatusEffect;


USTRUCT(BlueprintType)
struct FAbilityCost
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Cost", meta = (Categories = "Stats"))
	FGameplayTag StatTag;

	UPROPERTY(EditDefaultsOnly, Category = "Cost")
	float Amount = 0.f;
};