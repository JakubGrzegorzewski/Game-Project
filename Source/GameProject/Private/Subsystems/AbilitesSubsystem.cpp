// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/AbilitesSubsystem.h"

#include "Abilites/AbilityDefinition.h"

void UAbilitesSubsystem::RegisterAbility(UAbilityDefinition* Ability){
	if (!Ability || !Ability->AbilityTag.IsValid()) return;
	AbilityRegistry.Add(Ability->AbilityTag, Ability);
}

const UAbilityDefinition* UAbilitesSubsystem::FindAbilityDefinition(FGameplayTag AbilityTag) const{
	if (const TObjectPtr<UAbilityDefinition>* Found = AbilityRegistry.Find(AbilityTag)){
		return *Found;
	}
	return nullptr;
}

TArray<UAbilityDefinition*> UAbilitesSubsystem::GetAllRegisteredAbilities() const{
	TArray<UAbilityDefinition*> Result;
	Result.Reserve(AbilityRegistry.Num());
	for (const TPair<FGameplayTag, TObjectPtr<UAbilityDefinition>>& Pair : AbilityRegistry){
		Result.Add(Pair.Value);
	}
	return Result;
}
