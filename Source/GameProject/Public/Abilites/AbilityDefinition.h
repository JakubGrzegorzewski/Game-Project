// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Enums/AbilityTargetType.h"
#include "Structs/AbilityStruct.h"
#include "Structs/StatusEffectSpec.h"
#include "AbilityDefinition.generated.h"

class ARingWithEffect;
class AProjectileWithEffect;


UCLASS(BlueprintType)
class GAMEPROJECT_API UAbilityDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (Categories = "Abilities"))
	FGameplayTag AbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FAbilityCost Cost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float CooldownDuration = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTagContainer BlockedByTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	EAbilityTargetType TargetType = EAbilityTargetType::ActorTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (EditCondition = "TargetType != EAbilityTargetType::Self"))
	float Range = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (EditCondition = "TargetType == EAbilityTargetType::AreaAtLocation"))
	float AreaRadius = 250.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (EditCondition = "TargetType == EAbilityTargetType::AreaAtLocation"))
	float AreaLifeTime = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (EditCondition = "TargetType == EAbilityTargetType::AreaAtLocation"))
	TSubclassOf<ARingWithEffect> RingClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (EditCondition = "TargetType == EAbilityTargetType::Projectile"))
	TSubclassOf<AProjectileWithEffect> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TArray<FStatusEffectSpec> Effects;
};
