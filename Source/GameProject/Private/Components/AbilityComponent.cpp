// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AbilityComponent.h"

#include "Actors/RingWithEffect.h"
#include "Characters/BaseCharacter.h"
#include "Components/StatsComponent.h"
#include "Components/StatusEffectComponent.h"
#include "Engine/World.h"
#include "Subsystems/AbilitesSubsystem.h"
#include "TimerManager.h"
#include "Enums/AbilityTargetType.h"
#include "Structs/StatusEffectSpec.h"

UAbilityComponent::UAbilityComponent(){
	PrimaryComponentTick.bCanEverTick = false;
}

void UAbilityComponent::BeginPlay(){
	Super::BeginPlay();

	for (const UAbilityDefinition* Ability : GrantedAbilities){
		if (!Ability || !Ability->AbilityTag.IsValid()) continue;

		RuntimeStates.Add(Ability->AbilityTag, FAbilityRuntimeState());

		if (UGameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance() : nullptr){
			if (UAbilitesSubsystem* Registry = GameInstance->GetSubsystem<UAbilitesSubsystem>()){
				Registry->RegisterAbility(const_cast<UAbilityDefinition*>(Ability));
			}
		}
	}
}

const UAbilityDefinition* UAbilityComponent::FindAbility(FGameplayTag AbilityTag) const{
	for (const UAbilityDefinition* Ability : GrantedAbilities){
		if (Ability && Ability->AbilityTag == AbilityTag){
			return Ability;
		}
	}
	return nullptr;
}

bool UAbilityComponent::CanUseAbility(FGameplayTag AbilityTag, FText& OutReason) const{
	const UAbilityDefinition* Definition = FindAbility(AbilityTag);
	if (!Definition){
		OutReason = FText::FromString(TEXT("Ability not granted"));
		return false;
	}

	const ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwner());
	if (!Owner){
		OutReason = FText::FromString(TEXT("Invalid owner"));
		return false;
	}

	if (UStatusEffectComponent* StatusComp = Owner->GetStatusEffectComponent()){
		if (!Definition->BlockedByTags.IsEmpty() && StatusComp->HasAnyTag(Definition->BlockedByTags)){
			OutReason = FText::FromString(TEXT("Blocked by current state"));
			return false;
		}
	}

	if (const FAbilityRuntimeState* State = RuntimeStates.Find(AbilityTag)){
		if (GetWorld() && GetWorld()->GetTimeSeconds() < State->CooldownEndTime){
			OutReason = FText::FromString(TEXT("On cooldown"));
			return false;
		}
	}

	if (Definition->Cost.StatTag.IsValid()){
		if (UStatsComponent* Stats = Owner->GetStatsComponent()){
			if (Stats->GetStatValue(Definition->Cost.StatTag) < Definition->Cost.Amount){
				OutReason = FText::FromString(TEXT("Not enough resource"));
				return false;
			}
		}
	}

	return true;
}

bool UAbilityComponent::TryUseAbility(FGameplayTag AbilityTag, AActor* OptionalTarget){
	FText Reason;
	if (!CanUseAbility(AbilityTag, Reason)){
		UE_LOG(LogTemp, Log, TEXT("TryUseAbility(%s) failed: %s"), *AbilityTag.ToString(), *Reason.ToString());
		return false;
	}

	const UAbilityDefinition* Definition = FindAbility(AbilityTag);
	ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwner());
	if (!Definition || !Owner) return false;

	AActor* ResolvedTarget = nullptr;
	FVector ResolvedLocation = FVector::ZeroVector;
	if (!ResolveTarget(Definition, OptionalTarget, ResolvedTarget, ResolvedLocation)){
		UE_LOG(LogTemp, Log, TEXT("TryUseAbility(%s) failed: no valid target"), *AbilityTag.ToString());
		return false;
	}

	if (Definition->Cost.StatTag.IsValid()){
		if (UStatsComponent* Stats = Owner->GetStatsComponent()){
			Stats->UpdateStat(Definition->Cost.StatTag, -Definition->Cost.Amount);
		}
	}

	StartCooldown(Definition);

	switch (Definition->TargetType){
	case EAbilityTargetType::Self:
	case EAbilityTargetType::ActorTarget:
	{
		if (ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(ResolvedTarget)){
			if (UStatusEffectComponent* TargetStatus = TargetCharacter->GetStatusEffectComponent()){
				for (const FStatusEffectSpec& Spec : Definition->Effects){
					TargetStatus->ApplyEffect(Spec, Owner);
				}
			}
		}
		break;
	}
	case EAbilityTargetType::AreaAtLocation:
	{
		if (UWorld* World = GetWorld()){
			ARingWithEffect* Ring = World->SpawnActorDeferred<ARingWithEffect>(
				ARingWithEffect::StaticClass(),
				FTransform(FRotator::ZeroRotator, ResolvedLocation),
				Owner, nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			if (Ring){
				Ring->Radius = Definition->AreaRadius;
				Ring->LifeTime = Definition->AreaLifeTime;
				Ring->Effects = Definition->Effects;
				Ring->FinishSpawning(FTransform(FRotator::ZeroRotator, ResolvedLocation));
			}
		}
		break;
	}
	}

	return true;
}

bool UAbilityComponent::ResolveTarget(const UAbilityDefinition* Definition, AActor* OptionalTarget, AActor*& OutTarget, FVector& OutLocation) const{
	ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwner());
	if (!Owner || !GetWorld()) return false;

	switch (Definition->TargetType){
	case EAbilityTargetType::Self:
		OutTarget = Owner;
		OutLocation = Owner->GetActorLocation();
		return true;

	case EAbilityTargetType::ActorTarget:
	{
		if (OptionalTarget && IsValid(OptionalTarget)){
			OutTarget = OptionalTarget;
			OutLocation = OptionalTarget->GetActorLocation();
			return true;
		}

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Owner);
		const FVector Start = Owner->GetActorLocation();
		const FVector End = Start + Owner->GetActorForwardVector() * Definition->Range;
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params) && Hit.bBlockingHit && IsValid(Hit.GetActor())){
			OutTarget = Hit.GetActor();
			OutLocation = Hit.Location;
			return true;
		}
		return false;
	}

	case EAbilityTargetType::AreaAtLocation:
	{
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Owner);
		const FVector Start = Owner->GetActorLocation();
		const FVector End = Start + Owner->GetActorForwardVector() * Definition->Range;
		const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, Params);
		OutLocation = (bHit && Hit.bBlockingHit) ? Hit.Location : End;
		OutTarget = nullptr;
		return true;
	}
	}

	return false;
}

void UAbilityComponent::StartCooldown(const UAbilityDefinition* Definition){
	if (!Definition || !GetWorld()) return;

	FAbilityRuntimeState& State = RuntimeStates.FindOrAdd(Definition->AbilityTag);
	State.CooldownEndTime = GetWorld()->GetTimeSeconds() + Definition->CooldownDuration;

	OnAbilityAvailabilityChanged.Broadcast(Definition->AbilityTag, false);

	if (Definition->CooldownDuration > 0.f){
		FTimerHandle Handle;
		const FGameplayTag AbilityTag = Definition->AbilityTag;
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UAbilityComponent::NotifyAbilityReady, AbilityTag);
		GetWorld()->GetTimerManager().SetTimer(Handle, Delegate, Definition->CooldownDuration, false);
	} else {
		NotifyAbilityReady(Definition->AbilityTag);
	}
}

void UAbilityComponent::NotifyAbilityReady(FGameplayTag AbilityTag){
	OnAbilityAvailabilityChanged.Broadcast(AbilityTag, true);
}

float UAbilityComponent::GetCooldownRemaining(FGameplayTag AbilityTag) const{
	if (const FAbilityRuntimeState* State = RuntimeStates.Find(AbilityTag)){
		if (GetWorld()){
			return FMath::Max(0.f, static_cast<float>(State->CooldownEndTime - GetWorld()->GetTimeSeconds()));
		}
	}
	return 0.f;
}
