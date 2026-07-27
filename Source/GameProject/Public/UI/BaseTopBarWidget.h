// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Components/StatsComponent.h"
#include "BaseTopBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UBaseTopBarWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void OnStatChanged(FGameplayTag StatTag, float NewValue);
	virtual void NativeConstruct() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStatsComponent* StatsComponent;
	
	UFUNCTION(BlueprintNativeEvent)
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);
	void UpdateHealthBar_Implementation(float CurrentHealth, float MaxHealth);
};
