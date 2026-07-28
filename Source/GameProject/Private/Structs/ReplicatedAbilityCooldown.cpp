#include "Structs/ReplicatedAbilityCooldown.h"

#include "Components/AbilityComponent.h"

FReplicatedAbilityCooldown& FReplicatedAbilityCooldownArray::FindOrAddEntry(const FGameplayTag& AbilityTag)
{
	for (FReplicatedAbilityCooldown& Entry : Entries)
	{
		if (Entry.AbilityTag == AbilityTag)
		{
			return Entry;
		}
	}

	FReplicatedAbilityCooldown& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.AbilityTag = AbilityTag;
	return NewEntry;
}

void FReplicatedAbilityCooldown::PostReplicatedAdd(const FReplicatedAbilityCooldownArray& InArray)
{
	if (UAbilityComponent* Component = InArray.OwnerComponent.Get())
	{
		Component->NotifyReplicatedCooldownChanged(AbilityTag, CooldownEndTime);
	}
}

void FReplicatedAbilityCooldown::PostReplicatedChange(const FReplicatedAbilityCooldownArray& InArray)
{
	if (UAbilityComponent* Component = InArray.OwnerComponent.Get())
	{
		Component->NotifyReplicatedCooldownChanged(AbilityTag, CooldownEndTime);
	}
}
