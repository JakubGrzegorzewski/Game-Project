// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseTopBarWidget.h"

void UBaseTopBarWidget::OnStatChanged(FGameplayTag StatTag, float NewValue){
	if (StatTag == FGameplayTag::RequestGameplayTag(FName("Stats.Health"))){
		FStatsStruct HealthStat;
		StatsComponent->GetStatByTag(StatTag, HealthStat);
		UpdateHealthBar(NewValue, StatsComponent ? HealthStat.MaxValue : 0.f);
	}
}

void UBaseTopBarWidget::NativeConstruct(){
	Super::NativeConstruct();
	
	StatsComponent->OnStatChanged.AddDynamic(this, &UBaseTopBarWidget::OnStatChanged);
	FStatsStruct HealthStat;
	StatsComponent->GetStatByTag(FGameplayTag::RequestGameplayTag(FName("Stats.Health")), HealthStat);
	UpdateHealthBar(HealthStat.DefaultValue, HealthStat.MaxValue);
}

void UBaseTopBarWidget::UpdateHealthBar_Implementation(float CurrentHealth, float MaxHealth){
	
}
