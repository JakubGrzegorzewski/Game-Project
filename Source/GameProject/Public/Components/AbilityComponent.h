// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Abilites/AbilityDefinition.h"
#include "AbilityComponent.generated.h"

USTRUCT()
struct FAbilityRuntimeState
{
	GENERATED_BODY()

	double CooldownEndTime = 0.0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityAvailabilityChanged, FGameplayTag, AbilityTag, bool, bIsAvailable);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPROJECT_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TObjectPtr<UAbilityDefinition>> GrantedAbilities;


	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool CanUseAbility(FGameplayTag AbilityTag, FText& OutReason) const;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryUseAbility(FGameplayTag AbilityTag, AActor* OptionalTarget = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	float GetCooldownRemaining(FGameplayTag AbilityTag) const;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	const UAbilityDefinition* FindAbility(FGameplayTag AbilityTag) const;

	UPROPERTY(BlueprintAssignable, Category = "Abilities")
	FOnAbilityAvailabilityChanged OnAbilityAvailabilityChanged;

protected:
	virtual void BeginPlay() override;

private:
	bool ResolveTarget(const UAbilityDefinition* Definition, AActor* OptionalTarget, AActor*& OutTarget, FVector& OutLocation) const;
	void StartCooldown(const UAbilityDefinition* Definition);
	void NotifyAbilityReady(FGameplayTag AbilityTag);

	UPROPERTY()
	TMap<FGameplayTag, FAbilityRuntimeState> RuntimeStates;
};
