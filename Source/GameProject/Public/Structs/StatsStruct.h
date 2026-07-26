#pragma once

#include "CoreMinimal.h"
#include "StatsStruct.generated.h"

USTRUCT(BlueprintType)
struct FStatsStruct
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MinValue;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxValue;
	
	float CurrentValue;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float DefaultValue;
	
	FString ToString() const{
		return FString::Printf(TEXT("MinValue: %f, MaxValue: %f, CurrentValue: %f, DefaultValue: %f"), MinValue, MaxValue, CurrentValue, DefaultValue);
	}
};