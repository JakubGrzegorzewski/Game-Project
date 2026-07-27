// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Structs/AbilityStruct.h"
#include "Structs/StatusEffectSpec.h"
#include "StatusEffectComponent.generated.h"

class UStatusEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActiveEffectsChanged);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPROJECT_API UStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatusEffectComponent();

	UFUNCTION(BlueprintCallable, Category = "Effects")
	bool ApplyEffect(const FStatusEffectSpec& Spec, AActor* Instigator);

	
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void RemoveEffect(UStatusEffect* Effect);

	UFUNCTION(BlueprintCallable, Category = "Effects")
	void RemoveEffectsByTag(FGameplayTag EffectTag);


	void RemoveActiveEffectsConflictingWithTags(const FGameplayTagContainer& NewlyGrantedTags, const UStatusEffect* SkipEffect = nullptr);

	void NotifyEffectTagsGranted(const FGameplayTagContainer& Tags);
	void NotifyEffectTagsRevoked(const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintCallable, Category = "Effects")
	bool HasTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Effects")
	bool HasAnyTag(const FGameplayTagContainer& Tags) const;

	UFUNCTION(BlueprintCallable, Category = "Effects")
	TArray<UStatusEffect*> GetActiveEffects() const;

	UPROPERTY(BlueprintAssignable, Category = "Effects")
	FOnActiveEffectsChanged OnActiveEffectsChanged;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void RefreshEffectDuration(UStatusEffect* Effect, float NewDuration);

	UPROPERTY()
	TArray<TObjectPtr<UStatusEffect>> ActiveEffects;

	UPROPERTY()
	TMap<TObjectPtr<UStatusEffect>, FTimerHandle> EffectTickTimers;

	UPROPERTY()
	TMap<TObjectPtr<UStatusEffect>, FTimerHandle> EffectExpireTimers;
	
	TMap<FGameplayTag, int32> AppliedTagCounts;
};
