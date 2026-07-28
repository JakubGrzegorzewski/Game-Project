#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Structs/StatusEffectSpec.h"
#include "ReplicatedActiveEffect.generated.h"

class UStatusEffectComponent;
struct FReplicatedActiveEffectArray;

USTRUCT()
struct FReplicatedActiveEffect : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	int32 EffectId = 0;

	UPROPERTY()
	FStatusEffectSpec Spec;

	UPROPERTY()
	double ExpireTime = 0.0;

	void PostReplicatedAdd(const FReplicatedActiveEffectArray& InArray);
	void PreReplicatedRemove(const FReplicatedActiveEffectArray& InArray);
};

USTRUCT()
struct FReplicatedActiveEffectArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FReplicatedActiveEffect> Entries;

	UPROPERTY(NotReplicated)
	TWeakObjectPtr<UStatusEffectComponent> OwnerComponent;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FReplicatedActiveEffect>(Entries, DeltaParms, *this);
	}
};

template <>
struct TStructOpsTypeTraits<FReplicatedActiveEffectArray> : public TStructOpsTypeTraitsBase2<FReplicatedActiveEffectArray>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
