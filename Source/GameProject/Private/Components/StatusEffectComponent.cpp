// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatusEffectComponent.h"

#include "Abilites/StatusEffect.h"
#include "Characters/BaseCharacter.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

UStatusEffectComponent::UStatusEffectComponent(){
	PrimaryComponentTick.bCanEverTick = false;
}

bool UStatusEffectComponent::ApplyEffect(const FStatusEffectSpec& Spec, AActor* Instigator){
	ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwner());
	if (!Owner || !Spec.EffectClass){
		return false;
	}

	if (!Spec.RequiredAbsentTags.IsEmpty() && HasAnyTag(Spec.RequiredAbsentTags)){
		return false;
	}

	if (Spec.EffectTag.IsValid()){
		TArray<UStatusEffect*> ExistingOfSameKind;
		for (UStatusEffect* Effect : ActiveEffects){
			if (Effect && Effect->GetSpec().EffectTag == Spec.EffectTag){
				ExistingOfSameKind.Add(Effect);
			}
		}

		if (ExistingOfSameKind.Num() > 0){
			switch (Spec.StackingPolicy){
			case EEffectStackingPolicy::IgnoreIfActive:
				return false;
			case EEffectStackingPolicy::RefreshDuration:
				RefreshEffectDuration(ExistingOfSameKind[0], Spec.Duration);
				return true;
			case EEffectStackingPolicy::Stack:
				if (ExistingOfSameKind.Num() >= FMath::Max(1, Spec.MaxStacks)){
					RefreshEffectDuration(ExistingOfSameKind[0], Spec.Duration);
					return true;
				}
				break;
			}
		}
	}

	UStatusEffect* NewEffect = NewObject<UStatusEffect>(this, Spec.EffectClass);
	if (!NewEffect) return false;

	NewEffect->Initialize(Spec, Owner, Instigator);
	ActiveEffects.Add(NewEffect);
	NewEffect->OnApply();

	if (Spec.Period > 0.f){
		FTimerHandle TickHandle;
		FTimerDelegate TickDelegate = FTimerDelegate::CreateUObject(NewEffect, &UStatusEffect::OnTick);
		GetWorld()->GetTimerManager().SetTimer(TickHandle, TickDelegate, Spec.Period, true);
		EffectTickTimers.Add(NewEffect, TickHandle);
	}

	if (Spec.Duration > 0.f){
		FTimerHandle ExpireHandle;
		FTimerDelegate ExpireDelegate = FTimerDelegate::CreateUObject(this, &UStatusEffectComponent::RemoveEffect, NewEffect);
		GetWorld()->GetTimerManager().SetTimer(ExpireHandle, ExpireDelegate, Spec.Duration, false);
		EffectExpireTimers.Add(NewEffect, ExpireHandle);
		OnActiveEffectsChanged.Broadcast();
	} else {
		RemoveEffect(NewEffect);
	}

	return true;
}

void UStatusEffectComponent::RemoveEffect(UStatusEffect* Effect){
	if (!Effect) return;

	const int32 Index = ActiveEffects.IndexOfByKey(Effect);
	if (Index == INDEX_NONE) return; 

	if (FTimerHandle* TickHandle = EffectTickTimers.Find(Effect)){
		if (GetWorld()) GetWorld()->GetTimerManager().ClearTimer(*TickHandle);
		EffectTickTimers.Remove(Effect);
	}
	if (FTimerHandle* ExpireHandle = EffectExpireTimers.Find(Effect)){
		if (GetWorld()) GetWorld()->GetTimerManager().ClearTimer(*ExpireHandle);
		EffectExpireTimers.Remove(Effect);
	}

	Effect->OnRemove();
	ActiveEffects.RemoveAt(Index);
	OnActiveEffectsChanged.Broadcast();
}

void UStatusEffectComponent::RemoveEffectsByTag(FGameplayTag EffectTag){
	TArray<TObjectPtr<UStatusEffect>> Matching;
	for (UStatusEffect* Effect : ActiveEffects){
		if (Effect && Effect->GetSpec().EffectTag == EffectTag){
			Matching.Add(Effect);
		}
	}
	for (UStatusEffect* Effect : Matching){
		RemoveEffect(Effect);
	}
}

void UStatusEffectComponent::RemoveActiveEffectsConflictingWithTags(const FGameplayTagContainer& NewlyGrantedTags, const UStatusEffect* SkipEffect){
	if (NewlyGrantedTags.IsEmpty()) return;

	TArray<TObjectPtr<UStatusEffect>> Conflicting;
	for (UStatusEffect* Effect : ActiveEffects){
		if (!Effect || Effect == SkipEffect) continue;
		if (Effect->GetSpec().RequiredAbsentTags.HasAny(NewlyGrantedTags)){
			Conflicting.Add(Effect);
		}
	}
	for (UStatusEffect* Effect : Conflicting){
		RemoveEffect(Effect);
	}
}

void UStatusEffectComponent::NotifyEffectTagsGranted(const FGameplayTagContainer& Tags){
	for (const FGameplayTag& Tag : Tags){
		AppliedTagCounts.FindOrAdd(Tag)++;
	}
}

void UStatusEffectComponent::NotifyEffectTagsRevoked(const FGameplayTagContainer& Tags){
	for (const FGameplayTag& Tag : Tags){
		if (int32* Count = AppliedTagCounts.Find(Tag)){
			if (--(*Count) <= 0){
				AppliedTagCounts.Remove(Tag);
			}
		}
	}
}

bool UStatusEffectComponent::HasTag(FGameplayTag Tag) const{
	const int32* Count = AppliedTagCounts.Find(Tag);
	return Count && *Count > 0;
}

bool UStatusEffectComponent::HasAnyTag(const FGameplayTagContainer& Tags) const{
	for (const FGameplayTag& Tag : Tags){
		if (HasTag(Tag)) return true;
	}
	return false;
}

TArray<UStatusEffect*> UStatusEffectComponent::GetActiveEffects() const{
	TArray<UStatusEffect*> Result;
	Result.Reserve(ActiveEffects.Num());
	for (UStatusEffect* Effect : ActiveEffects){
		Result.Add(Effect);
	}
	return Result;
}


void UStatusEffectComponent::RefreshEffectDuration(UStatusEffect* Effect, float NewDuration){
	if (!Effect) return;
	Effect->RefreshDuration(NewDuration);

	if (FTimerHandle* Handle = EffectExpireTimers.Find(Effect)){
		FTimerDelegate ExpireDelegate = FTimerDelegate::CreateUObject(this, &UStatusEffectComponent::RemoveEffect, Effect);
		GetWorld()->GetTimerManager().SetTimer(*Handle, ExpireDelegate, NewDuration, false);
	} else if (NewDuration > 0.f && GetWorld()){
		FTimerHandle NewHandle;
		FTimerDelegate ExpireDelegate = FTimerDelegate::CreateUObject(this, &UStatusEffectComponent::RemoveEffect, Effect);
		GetWorld()->GetTimerManager().SetTimer(NewHandle, ExpireDelegate, NewDuration, false);
		EffectExpireTimers.Add(Effect, NewHandle);
	}
	OnActiveEffectsChanged.Broadcast();
}

void UStatusEffectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason){
	TArray<TObjectPtr<UStatusEffect>> EffectsCopy = ActiveEffects;
	for (UStatusEffect* Effect : EffectsCopy){
		RemoveEffect(Effect);
	}
	Super::EndPlay(EndPlayReason);
}
