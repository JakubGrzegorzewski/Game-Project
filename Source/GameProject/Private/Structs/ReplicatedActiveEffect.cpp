#include "Structs/ReplicatedActiveEffect.h"

#include "Components/StatusEffectComponent.h"

void FReplicatedActiveEffect::PostReplicatedAdd(const FReplicatedActiveEffectArray& InArray)
{
	if (UStatusEffectComponent* Component = InArray.OwnerComponent.Get())
	{
		Component->NotifyReplicatedEffectAdded(EffectId, Spec, ExpireTime);
	}
}

void FReplicatedActiveEffect::PreReplicatedRemove(const FReplicatedActiveEffectArray& InArray)
{
	if (UStatusEffectComponent* Component = InArray.OwnerComponent.Get())
	{
		Component->NotifyReplicatedEffectRemoved(EffectId);
	}
}
