#pragma once

#include "CoreMinimal.h"
#include "StatsStruct.generated.h"

USTRUCT(BlueprintType)
struct FStatsStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float MinValue = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float MaxValue = 100.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Stats")
	float CurrentValue = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float DefaultValue = 100.f;

	FString ToString() const{
		return FString::Printf(TEXT("MinValue: %f, MaxValue: %f, CurrentValue: %f, DefaultValue: %f"), MinValue, MaxValue, CurrentValue, DefaultValue);
	}
};
