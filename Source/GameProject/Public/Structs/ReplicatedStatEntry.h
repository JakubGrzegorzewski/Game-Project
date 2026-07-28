#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Structs/StatsStruct.h"
#include "ReplicatedStatEntry.generated.h"

class UStatsComponent;
struct FReplicatedStatArray;

USTRUCT()
struct FReplicatedStatEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag StatTag;

	UPROPERTY()
	FStatsStruct Stat;

	void PostReplicatedAdd(const FReplicatedStatArray& InArray);
	void PostReplicatedChange(const FReplicatedStatArray& InArray);
};

USTRUCT()
struct FReplicatedStatArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FReplicatedStatEntry> Entries;

	UPROPERTY(NotReplicated)
	TWeakObjectPtr<UStatsComponent> OwnerComponent;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FReplicatedStatEntry>(Entries, DeltaParms, *this);
	}

	FReplicatedStatEntry& FindOrAddEntry(const FGameplayTag& StatTag);
};

template <>
struct TStructOpsTypeTraits<FReplicatedStatArray> : public TStructOpsTypeTraitsBase2<FReplicatedStatArray>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
