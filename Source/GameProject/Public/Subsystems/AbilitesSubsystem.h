// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Abilites/AbilityDefinition.h"
#include "AbilitesSubsystem.generated.h"


UCLASS()
class GAMEPROJECT_API UAbilitesSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void RegisterAbility(UAbilityDefinition* Ability);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	const UAbilityDefinition* FindAbilityDefinition(FGameplayTag AbilityTag) const;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	TArray<UAbilityDefinition*> GetAllRegisteredAbilities() const;

private:
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UAbilityDefinition>> AbilityRegistry;
};
