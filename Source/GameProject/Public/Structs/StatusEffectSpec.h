#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Enums/EffectStackingPolicy.h"

#include "StatusEffectSpec.generated.h"

class UStatusEffect;

USTRUCT(BlueprintType)
struct FStatusEffectSpec
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect")
	TSubclassOf<UStatusEffect> EffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect", meta = (Categories = "Effect"))
	FGameplayTag EffectTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect")
	float Duration = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect")
	float Period = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect")
	float Magnitude = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect", meta = (Categories = "Stats"))
	FGameplayTag StatTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect")
	FGameplayTagContainer GrantedTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect")
	FGameplayTagContainer RequiredAbsentTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect")
	EEffectStackingPolicy StackingPolicy = EEffectStackingPolicy::RefreshDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect", meta = (EditCondition = "StackingPolicy == EEffectStackingPolicy::Stack"))
	int32 MaxStacks = 1;
};
