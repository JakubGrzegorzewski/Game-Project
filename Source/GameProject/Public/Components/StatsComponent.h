// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Structs/StatsStruct.h"
#include "StatsComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChanged, FGameplayTag, StatTag, float, NewValue);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPROJECT_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatsComponent();
	
	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (Categories = "Stats"))
	TMap <FGameplayTag, FStatsStruct> Stats;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateStat(UPARAM(meta=(Categories="Stats")) FGameplayTag StatTag, float NewValue);
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float ModifyStat(UPARAM(meta=(Categories="Stats")) FGameplayTag StatTag, float Delta);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetStatValue(UPARAM(meta=(Categories="Stats")) FGameplayTag StatTag) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	void GetStatByTag(UPARAM(meta=(Categories="Stats")) FGameplayTag StatTag, FStatsStruct& OutStat) const;
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	bool AddNewStat(UPARAM(meta=(Categories="Stats")) FGameplayTag StatTag, FStatsStruct NewStat);

	TMap<FGameplayTag, FStatsStruct> GetStats() const;

	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnStatChanged;

protected:
	virtual void BeginPlay() override;
};
