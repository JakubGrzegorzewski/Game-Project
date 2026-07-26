// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/StatusEffectSpec.h"
#include "StatusEffect.generated.h"

class ABaseCharacter;


UCLASS(Abstract, Blueprintable)
class GAMEPROJECT_API UStatusEffect : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(const FStatusEffectSpec& InSpec, ABaseCharacter* InTarget, AActor* InInstigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void OnApply();
	virtual void OnApply_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void OnTick();
	virtual void OnTick_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void OnRemove();
	virtual void OnRemove_Implementation();

	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintPure, Category = "Effect")
	FStatusEffectSpec GetSpec() const { return Spec; }

	UFUNCTION(BlueprintPure, Category = "Effect")
	ABaseCharacter* GetTargetCharacter() const { return TargetCharacter.Get(); }

	UFUNCTION(BlueprintPure, Category = "Effect")
	AActor* GetInstigator() const { return Instigator.Get(); }

	UFUNCTION(BlueprintPure, Category = "Effect")
	float GetTimeRemaining() const;

	void RefreshDuration(float NewDuration);

protected:
	UPROPERTY()
	FStatusEffectSpec Spec;

	UPROPERTY()
	TWeakObjectPtr<ABaseCharacter> TargetCharacter;

	UPROPERTY()
	TWeakObjectPtr<AActor> Instigator;

	double ExpireTime = 0.0;
};
