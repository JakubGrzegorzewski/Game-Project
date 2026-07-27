// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatsComponent.h"

#include "GameplayTagContainer.h"
#include "GameProject/Public/Structs/StatsStruct.h"

UStatsComponent::UStatsComponent(){
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatsComponent::BeginPlay(){
	Super::BeginPlay();

	for (TPair<FGameplayTag, FStatsStruct>& Pair : Stats)
	{
		UpdateStat(Pair.Key, Pair.Value.DefaultValue);
	}
}

void UStatsComponent::UpdateStat(const FGameplayTag StatTag, float NewValue){
	if (StatTag.IsValid() && Stats.Contains(StatTag)) {
		if (NewValue >= Stats[StatTag].MinValue && NewValue <= Stats[StatTag].MaxValue) {
			Stats[StatTag].CurrentValue = NewValue;
			OnStatChanged.Broadcast(StatTag, NewValue);
		} else {
			Stats[StatTag].CurrentValue = FMath::Clamp(NewValue, Stats[StatTag].MinValue, Stats[StatTag].MaxValue);

			UE_LOG(LogTemp, Warning, TEXT("NewValue is out of bounds for StatTag: %s, NewValue: %f"), *Stats[StatTag].ToString(), NewValue);
		}
	} else{
		UE_LOG(LogTemp, Warning, TEXT("StatTag is invalid or not found in Stats."));
	}
}

float UStatsComponent::ModifyStat(const FGameplayTag StatTag, float Delta){
	if (!StatTag.IsValid() || !Stats.Contains(StatTag)) {
		UE_LOG(LogTemp, Warning, TEXT("StatTag is invalid or not found in Stats."));
		return 0.0f;
	}

	FStatsStruct& Stat = Stats[StatTag];
	const float NewValue = FMath::Clamp(Stat.CurrentValue + Delta, Stat.MinValue, Stat.MaxValue);
	Stat.CurrentValue = NewValue;
	OnStatChanged.Broadcast(StatTag, NewValue);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Stat changed: %s, Value: %f"), *StatTag.ToString(), NewValue));
	return NewValue;
}

float UStatsComponent::GetStatValue(const FGameplayTag StatTag) const{
	if (StatTag.IsValid() && Stats.Contains(StatTag)) {
		return Stats[StatTag].CurrentValue;
	}

	UE_LOG(LogTemp, Warning, TEXT("StatTag is invalid or not found in Stats."));
	return 0.0f;

}

void UStatsComponent::GetStatByTag(FGameplayTag StatTag, FStatsStruct& OutStat) const{
	
	if (StatTag.IsValid() && Stats.Contains(StatTag)) {
		OutStat = Stats[StatTag];
	} else {
		UE_LOG(LogTemp, Warning, TEXT("StatTag is invalid or not found in Stats."));
	}
}

bool UStatsComponent::AddNewStat(const FGameplayTag StatTag, FStatsStruct NewStat){
	if (StatTag.IsValid() && !Stats.Contains(StatTag)) {
		NewStat.CurrentValue = NewStat.DefaultValue;
		Stats.Add(StatTag, NewStat);
		return true;
	} else {
		UE_LOG(LogTemp, Warning, TEXT("StatTag is invalid or already exists in Stats."));
		return false;
	}
}
TMap<FGameplayTag, FStatsStruct> UStatsComponent::GetStats() const{
	return Stats;
}
