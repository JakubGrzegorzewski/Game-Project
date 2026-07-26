// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilites/StatusEffect.h"

#include "Characters/BaseCharacter.h"
#include "Components/StatusEffectComponent.h"

void UStatusEffect::Initialize(const FStatusEffectSpec& InSpec, ABaseCharacter* InTarget, AActor* InInstigator){
	Spec = InSpec;
	TargetCharacter = InTarget;
	Instigator = InInstigator;
	ExpireTime = (InTarget && InTarget->GetWorld()) ? InTarget->GetWorld()->GetTimeSeconds() + Spec.Duration : 0.0;
}

void UStatusEffect::OnApply_Implementation(){
	if (ABaseCharacter* Target = TargetCharacter.Get()){
		if (UStatusEffectComponent* StatusComp = Target->GetStatusEffectComponent()){
			StatusComp->NotifyEffectTagsGranted(Spec.GrantedTags);
		}
	}
}

void UStatusEffect::OnTick_Implementation(){
}

void UStatusEffect::OnRemove_Implementation(){
	if (ABaseCharacter* Target = TargetCharacter.Get()){
		if (UStatusEffectComponent* StatusComp = Target->GetStatusEffectComponent()){
			StatusComp->NotifyEffectTagsRevoked(Spec.GrantedTags);
		}
	}
}

UWorld* UStatusEffect::GetWorld() const{
	return TargetCharacter.IsValid() ? TargetCharacter->GetWorld() : nullptr;
}

float UStatusEffect::GetTimeRemaining() const{
	if (Spec.Duration <= 0.f || !GetWorld()) return 0.f;
	return FMath::Max(0.f, static_cast<float>(ExpireTime - GetWorld()->GetTimeSeconds()));
}

void UStatusEffect::RefreshDuration(float NewDuration){
	Spec.Duration = NewDuration;
	if (GetWorld()){
		ExpireTime = GetWorld()->GetTimeSeconds() + NewDuration;
	}
}
