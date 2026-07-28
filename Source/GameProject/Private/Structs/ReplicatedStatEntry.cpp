#include "Structs/ReplicatedStatEntry.h"

#include "Components/StatsComponent.h"

FReplicatedStatEntry& FReplicatedStatArray::FindOrAddEntry(const FGameplayTag& StatTag)
{
	for (FReplicatedStatEntry& Entry : Entries)
	{
		if (Entry.StatTag == StatTag)
		{
			return Entry;
		}
	}

	FReplicatedStatEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.StatTag = StatTag;
	return NewEntry;
}

void FReplicatedStatEntry::PostReplicatedAdd(const FReplicatedStatArray& InArray)
{
	if (UStatsComponent* Component = InArray.OwnerComponent.Get())
	{
		Component->NotifyReplicatedStatChanged(StatTag, Stat.CurrentValue);
	}
}

void FReplicatedStatEntry::PostReplicatedChange(const FReplicatedStatArray& InArray)
{
	if (UStatsComponent* Component = InArray.OwnerComponent.Get())
	{
		Component->NotifyReplicatedStatChanged(StatTag, Stat.CurrentValue);
	}
}
