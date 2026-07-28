#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ReplicatedAbilityCooldown.generated.h"

class UAbilityComponent;
struct FReplicatedAbilityCooldownArray;

USTRUCT()
struct FReplicatedAbilityCooldown : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag AbilityTag;

	UPROPERTY()
	double CooldownEndTime = 0.0;

	void PostReplicatedAdd(const FReplicatedAbilityCooldownArray& InArray);
	void PostReplicatedChange(const FReplicatedAbilityCooldownArray& InArray);
};

USTRUCT()
struct FReplicatedAbilityCooldownArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FReplicatedAbilityCooldown> Entries;

	UPROPERTY(NotReplicated)
	TWeakObjectPtr<UAbilityComponent> OwnerComponent;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FReplicatedAbilityCooldown>(Entries, DeltaParms, *this);
	}

	FReplicatedAbilityCooldown& FindOrAddEntry(const FGameplayTag& AbilityTag);
};

template <>
struct TStructOpsTypeTraits<FReplicatedAbilityCooldownArray> : public TStructOpsTypeTraitsBase2<FReplicatedAbilityCooldownArray>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
